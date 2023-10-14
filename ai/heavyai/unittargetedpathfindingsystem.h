#pragma once

#include "game/unitpathfindingsystem.h"
#include "ai/heavyai/heavyAiSharedData.h"

class UnitTargetedPathFindingSystem;
using spUnitTargetedPathFindingSystem = std::shared_ptr<UnitTargetedPathFindingSystem>;

class UnitTargetedPathFindingSystem : public UnitPathFindingSystem
{
    Q_OBJECT
public:
    struct Target
    {
        Unit* pUnit{nullptr};
    };

    UnitTargetedPathFindingSystem(GameMap* pMap, qint32 unitIdx, std::array<HeavyAiSharedData::spUnitInfo, HeavyAiSharedData::UNIT_COUNT> & pTargets);
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
    std::array<HeavyAiSharedData::spUnitInfo, HeavyAiSharedData::UNIT_COUNT> & m_pTargets;
    qint32 m_searchIndex{0};
    bool m_allReached{false};
    qint32 m_unitIdx{-1};
};

