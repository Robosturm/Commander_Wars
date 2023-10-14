#pragma once

#include <QtTypes>

class Unit;
class UnitTargetedPathFindingSystem;
using spUnitTargetedPathFindingSystem = std::shared_ptr<UnitTargetedPathFindingSystem>;

namespace HeavyAiSharedData
{

static constexpr qint32 UNIT_COUNT = 40;
static constexpr qint32 SEARCH_RANGE = 30;

enum SituationFeatures
{
    HP,
    HpDamage,
    FundsDamage,
    MovementPoints,
    Distance,
    HasMoved,
    Defense,
    RepairsOnPosition,
    CapturePoints,
    BuildingImportance,
    Stealthed,
    MaxFeatures,
};

enum AiCache
{
    UnitMovementPoints,
    MinFirerange,
    MaxFirerange,
    TerrainDefense,
    CanMoveAndFire,
    MaxAiCache,
};

struct UnitInfo
{
    Unit* pUnit{nullptr};
    qint32 multiplier{0};
    std::array<bool, UNIT_COUNT> reachable{false};
    std::vector<bool> stealthed;
    spUnitTargetedPathFindingSystem pUnitTargetedPathFindingSystem;
};

using spUnitInfo = std::shared_ptr<UnitInfo>;

}
