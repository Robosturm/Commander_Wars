#include "targetedunitpathfindingsystem.h"

#include "coreengine/mainapp.h"

#include "ai/coreai.h"

#include "game/gamemap.h"

TargetedUnitPathFindingSystem::TargetedUnitPathFindingSystem(Unit* pUnit, QVector<QVector3D>& targets)
    : UnitPathFindingSystem (pUnit),
      m_Targets(targets)
{
    for (qint32 i = 0; i < m_Targets.size(); i++)
    {
        if (m_Targets[i].z() <= 0.0f)
        {
            m_Targets[i].setZ(1.0f);
        }
    }
}

qint32 TargetedUnitPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32)
{
    qint32 minCost = -1;
    for (qint32 i = 0; i < m_Targets.size(); i++)
    {
        qint32 cost = static_cast<qint32>(qAbs(static_cast<qint32>(m_Targets[i].x()) - x) +
                      qAbs(static_cast<qint32>(m_Targets[i].y()) - y) * m_Targets[i].z()) +
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
    return minCost;
}

QPoint TargetedUnitPathFindingSystem::getReachableTargetField(qint32 movepoints)
{
    if (m_FinishNode >= 0)
    {
        Node* pNode = m_ClosedList[m_FinishNode];
        while (pNode->previousNodes.size() > 0 &&
               pNode->currentCost > movepoints)
        {
            // use a random node?
            pNode = pNode->previousNodes[0];
        }
        return QPoint(pNode->x, pNode->y);
    }
    return QPoint(-1, -1);
}

bool TargetedUnitPathFindingSystem::finished(qint32 x, qint32 y, qint32 costs)
{
    qint32 index = CoreAI::index(m_Targets, QPoint(x, y));
    if (index >= 0)
    {
        m_FinishNodes.append(std::tuple<qint32, qint32, qint32, float>(x, y, costs, m_Targets[index].z()));
    }
    if (GameMap::getInstance()->getTerrain(x, y)->getUnit() == nullptr)
    {
        return (index >= 0);
    }
    return false;
}

void TargetedUnitPathFindingSystem::setFinishNode()
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
        m_FinishNode = PathFindingSystem::getNodeIndex(x, y);
    }
}
