#pragma once

#include <QtTypes>

class Unit;
class UnitTargetedPathFindingSystem;
using spUnitTargetedPathFindingSystem = std::shared_ptr<UnitTargetedPathFindingSystem>;

namespace HeavyAiSharedData
{

static constexpr qint32 UNIT_COUNT = 40;
static constexpr qint32 SEARCH_RANGE = 30;
static constexpr qint32 HQ_IMPORTANCE = 1024;

enum SituationFeatures
{
    Distance,
    HP,
    HpDamage,
    FundsDamage,
    MovementPoints,
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
    CanMoveAndFire,
    CurrentRepair,
    BaseCost,
    MaxAiCache,
};

struct UnitInfo
{
    Unit* pUnit{nullptr};
    qint32 multiplier{0};
    qint32 terrainDefense{0};
    qint32 buildingImportance{0};
    std::array<bool, UNIT_COUNT> reachable{false};
    std::vector<bool> stealthed;

    spUnitTargetedPathFindingSystem pUnitTargetedPathFindingSystem;
};

using spUnitInfo = std::shared_ptr<UnitInfo>;

}
