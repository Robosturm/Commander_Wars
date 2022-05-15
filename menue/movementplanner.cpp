#include "menue/movementplanner.h"
#include "game/gamemap.h"
#include "gameinput/moveplannerinput.h"

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

    m_Topbar->finishCreation();
    addChild(m_Topbar);
    connect(m_Topbar.get(), &Topbar::sigItemClicked, this, &MovementPlanner::clickedTopbar, Qt::QueuedConnection);
    connect(this, &MovementPlanner::sigExit, pOwner, &GameMenue::exitMovementPlanner, Qt::QueuedConnection);
    connect(this, &MovementPlanner::sigHide, pOwner, &GameMenue::hideMovementPlanner, Qt::QueuedConnection);

    if (m_pPlayerinfo.get())
    {
        m_pPlayerinfo->setVisible(false);
    }
    m_pMap->updateSprites();
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

    GameMap* pSourceMap = m_pOwner->getMap();
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    QDataStream stream(&buffer);
    pSourceMap->serializeObject(stream);
    buffer.seek(0);
    m_pMap->clearMap();
    m_pMap->deserializeObject(stream);
    m_pMap->setCurrentPlayer(m_pViewPlayer->getPlayerID());
    Player* pPlayer = m_pMap->getCurrentViewPlayer();
    pPlayer->setBaseGameInput(spMoveplannerInput::create(m_pMap.get()));
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

    }
}

void MovementPlanner::hide()
{
    emit sigHide();
}

void MovementPlanner::exit()
{
    emit sigExit();
}
