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
    Interpreter::setCppOwnerShip(this);
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
            // ignore unit if it's not an enemy unit or if it's stealthed
            if (m_pUnit->getOwner()->isEnemyUnit(pUnit) &&
               (!pUnit->isStealthed(m_pUnit->getOwner())))
            {
                return -1;
            }
        }
        qint32 baseCosts = pMovementTableManager->getMovementPoints(m_pUnit.get(), pMap->getTerrain(x, y));
        qint32 costs = baseCosts + m_pUnit->getOwner()->getMovementpointModifier(m_pUnit.get(), QPoint(x, y));
        if (baseCosts > 0)
        {
            if ((costs <= 0) && (baseCosts > 0))
            {
                return 1;
            }
            else
            {
                return costs;
            }
        }
        else
        {
            return -1;
        }
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
