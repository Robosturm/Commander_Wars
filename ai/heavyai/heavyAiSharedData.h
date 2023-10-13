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
    Max,
};

enum class AiCache
{
    MovementPoints,
    MinFirerange,
    MaxFirerange,
    CanMoveAndFire,
    Max,
};


struct UnitInfo
{
    Unit* pUnit{nullptr};
    qint32 multiplier{0};
    bool reachable{false};
};

}
