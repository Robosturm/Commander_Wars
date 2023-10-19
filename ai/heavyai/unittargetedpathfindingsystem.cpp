#include "ai/heavyai/unittargetedpathfindingsystem.h"
#include "coreengine/globalutils.h"

UnitTargetedPathFindingSystem::UnitTargetedPathFindingSystem(GameMap* pMap, qint32 unitIdx, std::array<HeavyAiSharedData::spUnitInfo, HeavyAiSharedData::UNIT_COUNT> & pTargets)
    : UnitPathFindingSystem(pMap, pTargets[unitIdx]->pUnit),
    m_pTargets(pTargets),
    m_unitIdx{unitIdx}

{
#ifdef GRAPHICSUPPORT
    setObjectName("UnitTargetedPathFindingSystem");
#endif
    Interpreter::setCppOwnerShip(this);
}

qint32 UnitTargetedPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32 currentCost)
{
    m_allReached = true;
    qint32 minCost = std::numeric_limits<qint32>::max();
    for (qint32 i = m_pTargets.size() - 1; i >= 0; --i)
    {
        auto & target = m_pTargets[i];
        if (target->pUnit != nullptr)
        {
            auto & cache = target->pUnit->getAiCache();
            qint32 distance = GlobalUtils::getDistance(x, y, target->pUnit->getX(), target->pUnit->getY());
            qint32 maxFireRange = cache[HeavyAiSharedData::AiCache::MaxFirerange];
            // in fire range?
            if (distance >= cache[HeavyAiSharedData::AiCache::MinFirerange] &&
                distance <= maxFireRange)
            {
                m_pTargets[m_unitIdx]->reachable[i] = true;
                if (i == m_searchIndex)
                {
                    --m_searchIndex;
                }
            }
            else
            {
                qint32 newCost = distance - maxFireRange;
                if (newCost < minCost)
                {
                    minCost = newCost;
                }
                m_allReached = false;
            }
        }
        else if (i == m_searchIndex)
        {
            --m_searchIndex;
        }
    }
    return minCost;
}

bool UnitTargetedPathFindingSystem::finished(qint32 x, qint32 y, qint32 movementCosts)
{
    return m_allReached;
}
