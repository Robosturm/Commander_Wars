#pragma once

#include <QtTypes>

class Unit;

namespace HeavyAiSharedData
{

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
    MaxFeatures,
};

enum AiCache
{
    UnitMovementPoints,
    MinFirerange,
    MaxFirerange,
    CanMoveAndFire,
    MaxAiCache,
};


struct UnitInfo
{
    Unit* pUnit{nullptr};
    qint32 multiplier{0};
    bool reachable{false};
};

}
