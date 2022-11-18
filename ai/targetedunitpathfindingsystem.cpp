#include "ai/targetedunitpathfindingsystem.h"
#include "ai/coreai.h"
#include "game/gamemap.h"

TargetedUnitPathFindingSystem::TargetedUnitPathFindingSystem(GameMap* pMap, Unit* pUnit, std::vector<QVector3D>& targets, std::vector<std::vector<std::tuple<qint32, bool>>>* pMoveCostMap)
    : UnitPathFindingSystem(pMap, pUnit),
      m_Targets(targets),
      m_pMoveCostMap(pMoveCostMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("TargetedUnitPathFindingSystem");
#endif
    Interpreter::setCppOwnerShip(this);
    setFast(true);
    for (auto & target : m_Targets)
    {
        if (target.z() <= 0.0f)
        {
            target.setZ(1.0f);
        }
    }
    setMovepoints(m_pUnit->getFuel() * 2);
}

qint32 TargetedUnitPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32 currentCost)
{
    qint32 minCost = -1;
    if (!m_abortOnCostExceed || currentCost <= m_Movepoints)
    {
        for (auto & target : m_Targets)
        {
            qint32 cost = static_cast<qint32>(qAbs(static_cast<qint32>(target.x()) - x) +
                                              qAbs(static_cast<qint32>(target.y()) - y)) * target.z() +
                          static_cast<qint32>(m_pUnit->getBaseMovementPoints() * (target.z() - 1.0));
            if (cost < minCost)
            {
                minCost = cost;
            }
            else if (minCost < 0)
            {
                minCost = cost;
            }
        }
    }
    return minCost;
}

qint32 TargetedUnitPathFindingSystem::getCosts(qint32 index, qint32 x, qint32 y, qint32 curX, qint32 curY, qint32 currentCost)
{
    qint32 costs = -1;
    costs = UnitPathFindingSystem::getCosts(index, x, y, curX, curY,currentCost);
    if (costs < 0) // not crossable
    {
        return -1;
    }
    if (currentCost > m_unitMovepoints)
    {
        Terrain* pTerrain = m_pMap->getTerrain(x, y);
        Unit* pUnit = pTerrain->getUnit();
        if (pUnit != nullptr)
        {
            constexpr qint32 blockCosts = 3;
            if (blockCosts > 3)
            {
                costs *= 2;
            }
            else
            {
                costs += blockCosts;
            }
        }
    }
    qint32 simulationCost = 0;
    if (m_pMoveCostMap != nullptr)
    {
        if ((m_pMoveCostMap->size() > x && x >= 0) &&
            (m_pMoveCostMap->at(x).size() > y && y >= 0))
        {
            simulationCost += std::get<0>(m_pMoveCostMap->at(x)[y]);
        }
    }
    costs += simulationCost;
    if (costs < 0)
    {
        costs = 0;
    }
    return costs;
}

bool TargetedUnitPathFindingSystem::getAbortOnCostExceed() const
{
    return m_abortOnCostExceed;
}

void TargetedUnitPathFindingSystem::setAbortOnCostExceed(bool abortOnCostExceed)
{
    m_abortOnCostExceed = abortOnCostExceed;
}

const std::vector<QVector3D> &TargetedUnitPathFindingSystem::getTargets() const
{
    return m_Targets;
}

QPoint TargetedUnitPathFindingSystem::getReachableTargetField(qint32 movepoints)
{
    if (m_FinishNode >= 0)
    {
        auto path = getPathFast(m_FinishNodeX, m_FinishNodeY);
        qint32 cost = UnitPathFindingSystem::getCosts(path);
        qint32 curX = m_FinishNodeX;
        qint32 curY = m_FinishNodeY;
        while (((curX != m_StartPoint.x() || curY != m_StartPoint.y()) &&
                (cost > movepoints)) ||
               (path.size() > 1 && UnitPathFindingSystem::getCosts(getIndex(curX, curY), curX, curY, path[1].x(), path[1].y(), 0) == 0))
        {
            path.erase(path.cbegin());
            cost = UnitPathFindingSystem::getCosts(path);
        }
        return path[0];
    }
    return QPoint(-1, -1);
}

bool TargetedUnitPathFindingSystem::finished(qint32 x, qint32 y, qint32 movementCosts)
{
    qint32 index = CoreAI::index(m_Targets, QPoint(x, y));
    if (index >= 0)
    {
        m_FinishNodes.push_back(std::tuple<qint32, qint32, qint32, float>(x, y, movementCosts, m_Targets[index].z()));
    }
    Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
    if ((pUnit == nullptr) ||
        m_pUnit->getIgnoreUnitCollision() ||
        pUnit->getOwner() == m_pUnit->getOwner())
    {
        return (index >= 0);
    }
    return false;
}

void TargetedUnitPathFindingSystem::setFinishNode(qint32, qint32)
{
    if (m_FinishNodes.size() > 0)
    {
        qint32 minCosts = std::numeric_limits<qint32>::max();
        qint32 x = -1;
        qint32 y = -1;
        for (auto & node : m_FinishNodes)
        {
            qint32 costs = static_cast<qint32>(std::get<2>(node) * std::get<3>(node));
            if (costs < minCosts)
            {
                minCosts = costs;
                x = std::get<0>(node);
                y = std::get<1>(node);
            }
        }
        PathFindingSystem::setFinishNode(x, y);
    }
}
