#include "game/unitpathfindingsystem.h"

#include "resource_management/movementtablemanager.h"

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "game/player.h"
#include "game/co.h"

UnitPathFindingSystem::UnitPathFindingSystem(spUnit pUnit)
    : PathFindingSystem(pUnit->getX(), pUnit->getY()),
      m_pUnit(pUnit)
{
}

qint32 UnitPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32 currentCost)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        return m_pUnit->getMovementPoints() - currentCost;
    }
    else
    {
        return -1;
    }
}

bool UnitPathFindingSystem::finished(qint32, qint32)
{
    return false;
}

qint32 UnitPathFindingSystem::getCosts(qint32 x, qint32 y)
{
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
        // check for an enemy on the field
        if (pUnit != nullptr)
        {
            if (m_pUnit->getOwner()->isEnemyUnit(pUnit))
            {
                return -1;
            }
        }
        return pMovementTableManager->getMovementPoints(m_pUnit.get(), pMap->getTerrain(x, y));
    }
    else
    {
        return -1;
    }
}

qint32 UnitPathFindingSystem::getCosts(QVector<QPoint> path)
{
    qint32 totalCosts = 0;
    for (qint32 i = 0; i < path.size(); i++)
    {
        totalCosts += getCosts(path[i].x(), path[i].y());
    }
    return totalCosts;
}
