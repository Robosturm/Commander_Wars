#include "ai/targetedunitpathfindingsystem.h"
#include "ai/coreai.h"
#include "coreengine/mainapp.h"
#include "game/gamemap.h"

TargetedUnitPathFindingSystem::TargetedUnitPathFindingSystem(Unit* pUnit, QVector<QVector3D>& targets, QVector<QVector<std::tuple<qint32, bool>>>* pMoveCostMap)
    : UnitPathFindingSystem (pUnit),
      m_Targets(targets),
      m_pMoveCostMap(pMoveCostMap)
{
    setObjectName("TargetedUnitPathFindingSystem");
    setFast(true);
    for (qint32 i = 0; i < m_Targets.size(); i++)
    {
        if (m_Targets[i].z() <= 0.0f)
        {
            m_Targets[i].setZ(1.0f);
        }
    }
}

qint32 TargetedUnitPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32 currentCost)
{
    qint32 minCost = -1;
    if (!m_abortOnCostExceed || currentCost <= m_Movepoints)
    {
        for (qint32 i = 0; i < m_Targets.size(); i++)
        {
            qint32 cost = static_cast<qint32>(qAbs(static_cast<qint32>(m_Targets[i].x()) - x) +
                                              qAbs(static_cast<qint32>(m_Targets[i].y()) - y)) * m_Targets[i].z() +
                          static_cast<qint32>(m_pUnit->getBaseMovementPoints() * (m_Targets[i].z() - 1.0));
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

qint32 TargetedUnitPathFindingSystem::getCosts(qint32 index, qint32 x, qint32 y, qint32 curX, qint32 curY)
{
    qint32 costs = -1;
    if (m_useBasecosts)
    {
        spGameMap pMap = GameMap::getInstance();
        if (pMap.get() != nullptr && pMap->onMap(x, y))
        {
            qint32 direction = getMoveDirection(curX, curY, x, y);
            QString id = pMap->getTerrain(curX, curY)->getID() + pMap->getTerrain(x, y)->getID() + "Base";
            bool found = m_costInfo.contains(id);
            if (found)
            {
                costs = m_costInfo[id];
                m_movecosts[index][direction] = costs;
            }
            else
            {
                costs = m_pUnit->getBaseMovementCosts(x, y, curX, curY);
                m_costInfo.insert(id, costs);
                m_movecosts[index][direction] = costs;
            }
        }
    }
    else
    {
        costs = UnitPathFindingSystem::getCosts(index, x, y, curX, curY);
    }
    if (costs < 0) // not crossable
    {
        return -1;
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

bool TargetedUnitPathFindingSystem::getUseBasecosts() const
{
    return m_useBasecosts;
}

void TargetedUnitPathFindingSystem::setUseBasecosts(bool useBasecosts)
{
    m_useBasecosts = useBasecosts;
}

bool TargetedUnitPathFindingSystem::getAbortOnCostExceed() const
{
    return m_abortOnCostExceed;
}

void TargetedUnitPathFindingSystem::setAbortOnCostExceed(bool abortOnCostExceed)
{
    m_abortOnCostExceed = abortOnCostExceed;
}

QPoint TargetedUnitPathFindingSystem::getReachableTargetField(qint32 movepoints)
{
    if (m_FinishNode >= 0)
    {
        QVector<QPoint> path = getPath(m_FinishNodeX, m_FinishNodeY);
        qint32 cost = UnitPathFindingSystem::getCosts(path);
        qint32 curX = m_FinishNodeX;
        qint32 curY = m_FinishNodeY;
        while (((curX != m_StartPoint.x() || curY != m_StartPoint.y()) &&
                (cost > movepoints)) ||
               (path.size() > 1 && UnitPathFindingSystem::getCosts(getIndex(curX, curY), curX, curY, path[1].x(), path[1].y()) == 0))
        {
            path.removeFirst();
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
        m_FinishNodes.append(std::tuple<qint32, qint32, qint32, float>(x, y, movementCosts, m_Targets[index].z()));
    }
    Unit* pUnit = GameMap::getInstance()->getTerrain(x, y)->getUnit();
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
        for (qint32 i = 0; i < m_FinishNodes.size(); i++)
        {
            qint32 costs = static_cast<qint32>(std::get<2>(m_FinishNodes[i]) * std::get<3>(m_FinishNodes[i]));
            if (costs < minCosts)
            {
                minCosts = costs;
                x = std::get<0>(m_FinishNodes[i]);
                y = std::get<1>(m_FinishNodes[i]);
            }
        }
        PathFindingSystem::setFinishNode(x, y);
    }
}
