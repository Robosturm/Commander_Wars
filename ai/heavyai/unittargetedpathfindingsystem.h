#pragma once

#include <game/unitpathfindingsystem.h>
#include "ai/heavyai/heavyAiSharedData.h"

class UnitTargetedPathFindingSystem : public UnitPathFindingSystem
{
    Q_OBJECT
public:
    struct Target
    {
        Unit* pUnit{nullptr};
    };

    UnitTargetedPathFindingSystem(GameMap* pMap, Unit* pUnit, std::vector<HeavyAiSharedData::UnitInfo> & pTargets);
    /**
     * @brief getRemainingCost
     * @param x
     * @param y
     * @return the remaining costs for this node to reach the target
     */
    virtual qint32 getRemainingCost(qint32 x, qint32 y, qint32 currentCost) override;
    /**
     * @brief finished checks if this would be the target node to reach
     * @param x
     * @param y
     * @param costs
     * @return
     */
    virtual bool finished(qint32 x, qint32 y, qint32)  override;
private:
    std::vector<HeavyAiSharedData::UnitInfo> m_pTargets;
    std::vector<HeavyAiSharedData::UnitInfo> m_reachableTargets;
};

