CO_WILL.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_WILL.activateSuperpower = function(co, powerMode, map)
{
	CO_WILL.activatePower(co, map);
};
CO_WILL.getSuperPowerDescription = function()
{
    return CO_WILL.getPowerDescription();
};
CO_WILL.getSuperPowerName = function()
{
    return CO_WILL.getPowerName();
};
CO_WILL.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    var seaAirUnit = (unit.getUnitType() === GameEnums.UnitType_Air) ||
                     (unit.getUnitType() === GameEnums.UnitType_Naval);
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (!seaAirUnit)
            {
                return 2;
            }
            return 0;
        default:
            return 0;
    }
};
CO_WILL.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    var seaAirUnit = (attacker.getUnitType() === GameEnums.UnitType_Air) ||
                     (attacker.getUnitType() === GameEnums.UnitType_Naval);
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
            {
                return 30;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
                {
                    return 30;
                }
                return 10;
            }
            break;
    }
    return 0;
};
