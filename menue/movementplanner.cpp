#include "menue/movementplanner.h"
#include "game/gamemap.h"

#include "coreengine/interpreter.h"

#include "resource_management/movementplanneraddinmanager.h"

#include "objects/loadingscreen.h"

#include "wiki/fieldinfo.h"

MovementPlanner::MovementPlanner(GameMenue* pOwner, Player* pViewPlayer)
    : GameMenue(spGameMap::create(1, 1, 2)),
      m_pOwner(pOwner),
      m_pViewPlayer(pViewPlayer)
{
    Interpreter::setCppOwnerShip(this);
    setObjectName("MovementPlanner");
    reloadMap();
    loadHandling();
    loadGameMenue();
    m_Topbar = spTopbar::create(0, Settings::getWidth() - m_IngameInfoBar->getScaledWidth());
    m_Topbar->addGroup(tr("Menu"));
    m_Topbar->addItem(tr("Hide"), "HIDE", 0, tr("Hides the movementplanner so you can move units and open it in the current state again."));
    m_Topbar->addItem(tr("Exit"), "EXIT", 0, tr("Quits the movementplanner. Opening the movementplanner again resets to the current map state."));

    MovementPlannerAddInManager* pMovementPlannerAddInManager = MovementPlannerAddInManager::getInstance();
    QStringList loadedGroups;
    for (qint32 i = 0; i < pMovementPlannerAddInManager->getCount(); ++i)
    {
        QString id = pMovementPlannerAddInManager->getID(i);
        addAddIn(loadedGroups, id);
    }
    m_Topbar->finishCreation();
    addChild(m_Topbar);
    connect(m_Topbar.get(), &Topbar::sigItemClicked, this, &MovementPlanner::clickedTopbar, Qt::QueuedConnection);
    connect(this, &MovementPlanner::sigExit, pOwner, &GameMenue::exitMovementPlanner, Qt::QueuedConnection);
    connect(this, &MovementPlanner::sigHide, pOwner, &GameMenue::hideMovementPlanner, Qt::QueuedConnection);
    connect(this, &MovementPlanner::sigLeftClick, this, &MovementPlanner::leftClick, Qt::QueuedConnection);

    if (m_pPlayerinfo.get())
    {
        m_pPlayerinfo->setVisible(false);
    }
    m_pMap->updateSprites();
}

void MovementPlanner::addAddIn(QStringList & loadedGroups, QString newAddInId)
{
    Interpreter* pInterpreter = Interpreter::getInstance();;
    bool valid = true;
    QString name;
    QString  groupName;
    QString  description;
    QJSValue erg = pInterpreter->doFunction(newAddInId, "getName");
    if (erg.isString() && valid)
    {
        name = erg.toString();
    }
    else
    {
        valid = false;
    }
    erg = pInterpreter->doFunction(newAddInId, "getGroupName");
    if (erg.isString() && valid)
    {
        groupName = erg.toString();
    }
    else
    {
        valid = false;
    }
    erg = pInterpreter->doFunction(newAddInId, "getDescription");
    if (erg.isString() && valid)
    {
        description = erg.toString();
    }
    else
    {
        valid = false;
    }
    if (valid)
    {
        qint32 index = loadedGroups.indexOf(groupName);
        if (index < 0)
        {
            index = loadedGroups.size();
            loadedGroups.append(groupName);
            m_Topbar->addGroup(groupName);
        }
        m_Topbar->addItem(name, newAddInId, index + 1, description);
    }
    auto addIn = spMovementPlannerAddIn::create(newAddInId);
    m_addIns.append(addIn);
}

void MovementPlanner::onShowPlanner()
{
    m_animationSkipper.storeAnimationSettings();
    m_animationSkipper.startSeeking();
}

void MovementPlanner::onExitPlanner()
{
    m_animationSkipper.restoreAnimationSettings();
}

Player *MovementPlanner::getViewPlayer() const
{
    return m_pViewPlayer;
}

void MovementPlanner::victory(qint32 team)
{
}

void MovementPlanner::reloadMap()
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->show();

    GameMap* pSourceMap = m_pOwner->getMap();
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    QDataStream stream(&buffer);
    pSourceMap->serializeObject(stream);
    buffer.seek(0);
    m_pMap->clearMap();
    m_pMap->deserializeObject(stream);
    pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->show();
    pLoadingScreen->setProgress(tr("Updating map"), 99);
    m_pMap->setCurrentPlayer(m_pViewPlayer->getPlayerID());
    Player* pPlayer = m_pMap->getCurrentViewPlayer();
    m_input = spMoveplannerInput::create(m_pMap.get());
    pPlayer->setBaseGameInput(m_input);
    pPlayer->getBaseGameInput()->init(this);
    m_pMap->getGameRules()->createFogVision();
    qint32 mapWidth = m_pMap->getMapWidth();
    qint32 mapHeight = m_pMap->getMapHeight();
    for (qint32 x = 0; x < mapWidth; ++x)
    {
        for (qint32 y = 0; y < mapHeight; ++y)
        {
            if (!pPlayer->getFieldVisible(x, y))
            {
                auto * pTerrain = m_pMap->getTerrain(x, y);
                auto * pBuilding = pTerrain->getBuilding();
                auto * pUnit = pTerrain->getUnit();
                if (pBuilding != nullptr && pBuilding->isEnemyBuilding(pPlayer))
                {
                    pBuilding->setOwner(nullptr);
                }
                if (pUnit != nullptr && pPlayer->isEnemyUnit(pUnit))
                {
                    pTerrain->setUnit(spUnit());
                }
            }
        }
    }
    pLoadingScreen->hide();
}

void MovementPlanner::clickedTopbar(QString itemID)
{
    CONSOLE_PRINT("clickedTopbar(" + itemID + ")", Console::eDEBUG);
    struct MenuItem
    {
        MenuItem(const char* const id, void (MovementPlanner::*func)())
            : m_Id(id),
              m_func(func)
        {
        }
        const char* const m_Id;
        void (MovementPlanner::*m_func)();
    };
    QVector<MenuItem> items =
    {
        MenuItem("EXIT",                &MovementPlanner::exit),
        MenuItem("HIDE",                &MovementPlanner::hide),
    };
    bool called = false;
    for (auto & item : qAsConst(items))
    {
        if (item.m_Id == itemID &&
            item.m_func != nullptr)
        {
            (this->*(item.m_func))();
            called = true;
            break;
        }
    }
    if (!called)
    {
        startAddIn(itemID);
    }
}

void MovementPlanner::hide()
{
    m_activeAddIn = nullptr;
    emit sigHide();
}

void MovementPlanner::exit()
{
    emit sigExit();
}

void MovementPlanner::startAddIn(QString addInId)
{
    m_input->setFocus(false);
    for (auto & addIn : m_addIns)
    {
        if (addIn->getAddIn() == addInId)
        {
            Interpreter* pInterpreter = Interpreter::getInstance();
            QJSValueList args({pInterpreter->newQObject(addIn.get()),
                               pInterpreter->newQObject(m_pMap.get())});
            pInterpreter->doFunction(addInId, "startAddIn", args);
            m_activeAddIn = addIn;
            break;
        }
    }
}

void MovementPlanner::leftClick(qint32 x, qint32 y)
{
    if (m_activeAddIn.get() != nullptr)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({pInterpreter->newQObject(m_activeAddIn.get()),
                           x,
                           y,
                           pInterpreter->newQObject(m_pMap.get())});
        pInterpreter->doFunction(m_activeAddIn->getAddIn(), "readyToExecute", args);
        execute();
    }
}

void MovementPlanner::execute()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(m_activeAddIn.get()),
                       pInterpreter->newQObject(m_pMap.get())});
    if (readyToExecute())
    {
        QJSValue erg = pInterpreter->doFunction(m_activeAddIn->getAddIn(), "execute", args);
        if (erg.isBool() && erg.toBool())
        {
            m_activeAddIn = nullptr;
        }
    }
    else
    {
        QJSValue erg = pInterpreter->doFunction(m_activeAddIn->getAddIn(), "nextInputType", args);
        GameEnums::AddinStepType type = static_cast<GameEnums::AddinStepType>(erg.toInt());
        if (type == GameEnums::AddinStepType_Menu)
        {

        }
    }
}

bool MovementPlanner::readyToExecute()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(m_activeAddIn.get()),
                       pInterpreter->newQObject(m_pMap.get())});
    QJSValue erg = pInterpreter->doFunction(m_activeAddIn->getAddIn(), "readyToExecute", args);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    return false;
}

void MovementPlanner::keyInput(oxygine::KeyEvent event)
{
    if (!event.getContinousPress())
    {
        // for debugging
        Qt::Key cur = event.getKey();
        if (m_Focused)
        {
            if (cur == Settings::getKey_information() ||
                     cur == Settings::getKey_information2())
            {

                Player* pPlayer = m_pMap->getCurrentViewPlayer();
                GameEnums::VisionType visionType = pPlayer->getFieldVisibleType(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                if (m_pMap->onMap(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()) &&
                    visionType != GameEnums::VisionType_Shrouded)
                {
                    Terrain* pTerrain = m_pMap->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                    Unit* pUnit = pTerrain->getUnit();
                    if (pUnit != nullptr && pUnit->isStealthed(pPlayer))
                    {
                        pUnit = nullptr;
                    }
                    spFieldInfo fieldinfo = spFieldInfo::create(pTerrain, pUnit);
                    addChild(fieldinfo);
                    connect(fieldinfo.get(), &FieldInfo::sigFinished, this, [this]
                    {
                        setFocused(true);
                    });
                    setFocused(false);
                }
            }
        }
    }
    InGameMenue::keyInput(event);
}
