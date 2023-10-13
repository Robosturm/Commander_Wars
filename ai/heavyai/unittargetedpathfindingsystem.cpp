#include "ai/heavyai/unittargetedpathfindingsystem.h"
#include "coreengine/globalutils.h"

UnitTargetedPathFindingSystem::UnitTargetedPathFindingSystem(GameMap* pMap, Unit* pUnit, std::vector<HeavyAiSharedData::UnitInfo> & pTargets)
    : UnitPathFindingSystem(pMap, pUnit),
    m_pTargets(pTargets)

{
#ifdef GRAPHICSUPPORT
    setObjectName("UnitTargetedPathFindingSystem");
#endif
    Interpreter::setCppOwnerShip(this);
    m_reachableTargets.reserve(m_pTargets.size());
}

qint32 UnitTargetedPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32 currentCost)
{
    qint32 minCost = std::numeric_limits<qint32>::max();
    for (qint32 i = 0; i < m_pTargets.size(); ++i)
    {
        auto & target = m_pTargets[i];
        qint32 distance = GlobalUtils::getDistance(x, y, target.pUnit->getX(), target.pUnit->getY());
        qint32 maxFireRange = target.pUnit->getAiCache()[HeavyAiSharedData::AiCache::MaxFirerange];
        // in fire range?
        if (distance >= target.pUnit->getAiCache()[HeavyAiSharedData::AiCache::MinFirerange] &&
            distance <= maxFireRange)
        {
            m_pTargets[i].reachable = true;
            m_reachableTargets.push_back(m_pTargets[i]);
            m_pTargets.erase(m_pTargets.cbegin() + i);
        }
        else
        {
            qint32 newCost = distance - maxFireRange;
            if (newCost < minCost)
            {
                minCost = newCost;
            }
        }
    }
    return minCost;
}

bool UnitTargetedPathFindingSystem::finished(qint32 x, qint32 y, qint32 movementCosts)
{
    return m_pTargets.size() == 0;
}
