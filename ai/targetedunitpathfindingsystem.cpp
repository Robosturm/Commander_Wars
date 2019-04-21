#include "targetedunitpathfindingsystem.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

TargetedUnitPathFindingSystem::TargetedUnitPathFindingSystem(Unit* pUnit, QVector<QPoint>& targets)
    : UnitPathFindingSystem (pUnit),
      m_Targets(targets)
{

}

qint32 TargetedUnitPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32)
{
    qint32 minCost = -1;
    for (qint32 i = 0; i < m_Targets.size(); i++)
    {
        qint32 cost = qAbs(m_Targets[i].x() - x) + qAbs(m_Targets[i].y() - y);
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

bool TargetedUnitPathFindingSystem::finished(qint32 x, qint32 y)
{
    return m_Targets.contains(QPoint(x, y));
}
