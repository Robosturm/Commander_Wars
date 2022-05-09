#include "menue/movementplanner.h"
#include "game/gamemap.h"

MovementPlanner::MovementPlanner(GameMenue* pOwner, GameMap* pSourceMap, Player* pViewPlayer)
    : GameMenue(spGameMap::create(1, 1, 2)),
      m_pOwner(pOwner)
{
    Interpreter::setCppOwnerShip(this);
    setObjectName("MovementPlanner");

    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    QDataStream stream(&buffer);
    pSourceMap->serializeObject(stream);
    buffer.seek(0);
    m_pMap->clearMap();
    m_pMap->deserializeObject(stream);
    m_pMap->setCurrentPlayer(pViewPlayer->getPlayerID());
    m_pMap->getGameRules()->createFogVision();
    Player* pPlayer = m_pMap->getCurrentViewPlayer();
    qint32 mapWidth = m_pMap->getMapWidth();
    qint32 mapHeight = m_pMap->getMapHeight();
    for (qint32 x = 0; x < mapWidth; ++x)
    {
        for (qint32 y = 0; y < mapHeight; ++y)
        {
            if (pPlayer->getFieldVisible(x, y))
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
    loadHandling();
    loadGameMenue();
    if (m_pPlayerinfo.get())
    {
        m_pPlayerinfo->setVisible(false);
    }
    m_pMap->updateSprites();
}

void MovementPlanner::onShowPlanner()
{
    m_animationSkipper.storeAnimationSettings();
}

void MovementPlanner::onExitPlanner()
{
    m_animationSkipper.restoreAnimationSettings();
}
