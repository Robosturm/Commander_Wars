#pragma once

#include <QtTypes>

class Unit;
class UnitTargetedPathFindingSystem;
using spUnitTargetedPathFindingSystem = std::shared_ptr<UnitTargetedPathFindingSystem>;

namespace HeavyAiSharedData
{

static constexpr qint32 UNIT_COUNT = 50;
static constexpr qint32 UNIT_SEARCH_RANGE = 40; // search range for the situation evaluator
static constexpr qint32 TILE_SEARCH_RANGE = 20; // search range for selecting on which tile
static constexpr qint32 HQ_IMPORTANCE = 1024;

enum SituationFeatures
{
    SituationFeatures_Distance,
    SituationFeatures_HP,
    SituationFeatures_HpDamage,
    SituationFeatures_FundsDamage,
    SituationFeatures_MovementPoints,
    SituationFeatures_HasMoved,
    SituationFeatures_Defense,
    SituationFeatures_RepairsOnPosition,
    SituationFeatures_CapturePoints,
    SituationFeatures_BuildingImportance,
    SituationFeatures_Stealthed,
    SituationFeatures_MinFireRange,
    SituationFeatures_MaxFireRange,
    SituationFeatures_CaBonus,
    SituationFeatures_MaxFeatures,
};

enum BuildingFeatures
{
    BuildingFeatures_RemainingGroundProductions,
    BuildingFeatures_RemainingSeaProductions,
    BuildingFeatures_RemainingAirProductions,
    BuildingFeatures_BuildUnitsThisTurn,
    BuildingFeatures_Funds,
    BuildingFeatures_Costs,
    BuildingFeatures_AverageDealingDamage,
    BuildingFeatures_AverageReceivingDamage,
    BuildingFeatures_AverageDistance,
    BuildingFeatures_CounteringUnits,
    BuildingFeatures_CoBonus,
    BuildingFeatures_MovementPoints,
    BuildingFeatures_CanMoveAndFire,
    BuildingFeatures_MinFireRange,
    BuildingFeatures_MaxFireRange,
    BuildingFeatures_CurrentDay,
    BuildingFeatures_CanCapture,
    BuildingFeatures_OwnUnitCount,
    BuildingFeatures_EnemyUnitCount,
};

enum UnitSelectFeatures
{
    UnitSelectFeatures_MovementPoints,
    UnitSelectFeatures_Costs,
    UnitSelectFeatures_MinFireRange,
    UnitSelectFeatures_MaxFireRange,
    UnitSelectFeatures_CanMoveAndFire,
    UnitSelectFeatures_EnemiesInRange,
    UnitSelectFeatures_AlliesNearby,
};

enum TileSelectFeatures
{
    TileSelectFeatures_TerrainDefense,
    TileSelectFeatures_CanReach,
    TileSelectFeatures_Movecost,
    TileSelectFeatures_AverageReceivingDamage,
    TileSelectFeatures_EnemiesInRange,
    TileSelectFeatures_NeededPowerCharge,
    TileSelectFeatures_Building
};

enum SituationOutput
{
    SituationOutput_Lost = -1,
    SituationOutput_Draw = 0,
    SituationOutput_Win = 1,
};

enum AiCache
{
    UnitMovementPoints,
    MinFirerange,
    MaxFirerange,
    CanMoveAndFire,
    CurrentRepair,
    BaseCost,
    CoBonus,
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
    void clear()
    {
        pUnit = nullptr;
        multiplier = 0;
        terrainDefense = 0;
        buildingImportance = 0;
        reachable.fill(false);
        stealthed.clear();
        pUnitTargetedPathFindingSystem.reset();
    }
};

static constexpr qint32 INPUT_VECTOR_SIZE = HeavyAiSharedData::UNIT_COUNT * HeavyAiSharedData::UNIT_COUNT * HeavyAiSharedData::SituationFeatures::SituationFeatures_MaxFeatures;

using spUnitInfo = std::shared_ptr<UnitInfo>;

}
