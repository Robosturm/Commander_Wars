#include "menue/movementplanner.h"
#include "game/gamemap.h"

#include "coreengine/interpreter.h"
#include "coreengine/console.h"

#include "ui_reader/uifactory.h"

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
    registerAtInterpreter("planner");
    reloadMap();
    loadHandling();
    loadGameMenue();
    m_Topbar = spTopbar::create(0, Settings::getWidth() - m_IngameInfoBar->getScaledWidth());
    QStringList loadedGroups;
    loadedGroups.append(tr("Menu"));
    m_Topbar->addGroup(loadedGroups[0]);
    MovementPlannerAddInManager* pMovementPlannerAddInManager = MovementPlannerAddInManager::getInstance();
    for (qint32 i = 0; i < pMovementPlannerAddInManager->getCount(); ++i)
    {
        QString id = pMovementPlannerAddInManager->getID(i);
        addAddIn(loadedGroups, id);
    }
    m_Topbar->addItem(tr("Hide"), "HIDE", 0, tr("Hides the movementplanner so you can move units and open it in the current state again."));
    m_Topbar->addItem(tr("Exit"), "EXIT", 0, tr("Quits the movementplanner. Opening the movementplanner again resets to the current map state."));
    m_Topbar->finishCreation();
    addChild(m_Topbar);
    connect(m_Topbar.get(), &Topbar::sigItemClicked, this, &MovementPlanner::clickedTopbar, Qt::QueuedConnection);
    connect(this, &MovementPlanner::sigExit, pOwner, &GameMenue::exitMovementPlanner, Qt::QueuedConnection);
    connect(this, &MovementPlanner::sigHide, pOwner, &GameMenue::hideMovementPlanner, Qt::QueuedConnection);
    connect(this, &MovementPlanner::sigLeftClick, this, &MovementPlanner::leftClick, Qt::QueuedConnection);
    connect(this, &MovementPlanner::sigRightClick, this, &MovementPlanner::rightClick, Qt::QueuedConnection);
    connect(&m_actionPerformer, &ActionPerformer::actionPerformed, this, &MovementPlanner::updateUpdateAddIns, Qt::QueuedConnection);

    if (m_pPlayerinfo.get())
    {
        m_pPlayerinfo->setVisible(false);
    }
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
        m_Topbar->addItem(name, newAddInId, index, description);
    }
    auto addIn = spMovementPlannerAddIn::create(newAddInId, m_pMap.get(), this);
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
    m_pMap->updateSprites();
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
            m_activeAddIn = addIn;
            Interpreter* pInterpreter = Interpreter::getInstance();
            QJSValueList args({pInterpreter->newQObject(addIn.get()),
                               pInterpreter->newQObject(m_pMap.get()),
                               pInterpreter->newQObject(this)});
            pInterpreter->doFunction(addInId, "startAddIn", args);
            changeCursor();
            execute();
            break;
        }
    }
}

void MovementPlanner::changeCursor()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getStepCursor";
    CursorData data;
    QJSValueList args({pInterpreter->newQObject(m_activeAddIn.get()),
                       pInterpreter->newQObject(&data),
                       pInterpreter->newQObject(m_pMap.get()),
                       pInterpreter->newQObject(this)});
    QJSValue ret = pInterpreter->doFunction(m_activeAddIn->getAddIn(), function1, args);
    if (ret.isString())
    {
        QString cursor = ret.toString();
        if (!cursor.isEmpty())
        {
            data.setCursor(ret.toString());
        }
    }
    getCursor()->changeCursor(data.getCursor(), data.getXOffset(), data.getYOffset(), data.getScale());
}

void MovementPlanner::leftClick(qint32 x, qint32 y)
{
    if (m_activeAddIn.get() != nullptr)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({pInterpreter->newQObject(m_activeAddIn.get()),
                           x,
                           y,
                           pInterpreter->newQObject(m_pMap.get()),
                           pInterpreter->newQObject(this)});
        pInterpreter->doFunction(m_activeAddIn->getAddIn(), "onFieldSelected", args);
        execute();
    }
}

void MovementPlanner::rightClick(qint32 x, qint32 y)
{
    stopAddIn();
}

void MovementPlanner::execute()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(m_activeAddIn.get()),
                       pInterpreter->newQObject(m_pMap.get()),
                       pInterpreter->newQObject(this)});
    if (readyToExecute())
    {
        CONSOLE_PRINT("Executing active addin " + m_activeAddIn->getAddIn(), Console::eDEBUG);
        QJSValue erg = pInterpreter->doFunction(m_activeAddIn->getAddIn(), "execute", args);
        updateUpdateAddIns();
        if ((erg.isBool() && erg.toBool()) ||
            !erg.isBool())
        {
            stopAddIn();
        }
    }
    else
    {
        QJSValue erg = pInterpreter->doFunction(m_activeAddIn->getAddIn(), "nextInputType", args);
        GameEnums::AddinStepType type = static_cast<GameEnums::AddinStepType>(erg.toInt());
        if (type == GameEnums::AddinStepType_Menu)
        {
            showAddInUi();
        }
    }
}

void MovementPlanner::showAddInUi()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(m_activeAddIn.get()),
                       pInterpreter->newQObject(m_pMap.get()),
                       pInterpreter->newQObject(this)});
    QJSValue erg = pInterpreter->doFunction(m_activeAddIn->getAddIn(), "getUiXml", args);
    if (erg.isString())
    {
        QString path = erg.toString();
        m_activeAddIn->show();
        UiFactory::getInstance().createUi(path, m_activeAddIn.get());
    }
    else
    {
        stopAddIn();
    }
}

bool MovementPlanner::readyToExecute()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(m_activeAddIn.get()),
                       pInterpreter->newQObject(m_pMap.get()),
                       pInterpreter->newQObject(this)});
    QJSValue erg = pInterpreter->doFunction(m_activeAddIn->getAddIn(), "readyToExecute", args);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    return false;
}

void MovementPlanner::onMenuInputDone()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(m_activeAddIn.get()),
                       pInterpreter->newQObject(m_pMap.get()),
                       pInterpreter->newQObject(this)});
    pInterpreter->doFunction(m_activeAddIn->getAddIn(), "onMenuInputDone", args);
    execute();
}

void MovementPlanner::stopAddIn()
{
    if (m_activeAddIn.get() != nullptr)
    {
        m_input->setFocus(true);
        bool isUpdateIn = false;
        for (qint32 i = 0; i < m_updateAddIns.size(); ++i)
        {
            if (m_updateAddIns[i] == m_activeAddIn.get())
            {
                isUpdateIn = true;
                break;
            }
        }
        if (!isUpdateIn)
        {
            m_activeAddIn->removeAllSprites();
        }
        m_activeAddIn->detach();
        m_activeAddIn = nullptr;
        CursorData data;
        getCursor()->changeCursor(data.getCursor(), data.getXOffset(), data.getYOffset(), data.getScale());
    }
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
    BaseGamemenu::keyInput(event);
}

void MovementPlanner::addActiveAddInToUpdateAddIns()
{
    if (!m_updateAddIns.contains(m_activeAddIn.get()))
    {
        m_updateAddIns.append(m_activeAddIn.get());
    }
}

void MovementPlanner::removeActiveAddInFromUpdateAddIns()
{
    for (qint32 i = 0; i < m_updateAddIns.size(); ++i)
    {
        if (m_updateAddIns[i] == m_activeAddIn.get())
        {
            m_updateAddIns.removeAt(i);
        }
    }
}

void MovementPlanner::updateUpdateAddIns()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (auto addIn : m_updateAddIns)
    {
        QJSValueList args({pInterpreter->newQObject(addIn),
                           pInterpreter->newQObject(m_pMap.get()),
                           pInterpreter->newQObject(this)});
        pInterpreter->doFunction(addIn->getAddIn(), "update", args);
    }
}
