CO_WILL.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_WILL.activateSuperpower = function(co, powerMode, map)
{
    CO_WILL.activatePower(co, powerMode);
};
CO_WILL.getSuperPowerDescription = function()
{
    return CO_WILL.getPowerDescription();
};
CO_WILL.getSuperPowerName = function()
{
    return CO_WILL.getPowerName();
};
CO_WILL.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
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
                return 20;
            }
            return 0;
        default:
            if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
            {
                return 10;
            }
            break;
        }
    }
    return 0;
};

CO_WILL.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
    }
    return 0;
};

CO_WILL.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        var seaAirUnit = (attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (!seaAirUnit)
            {
                return 1;
            }
            return 0;
        default:
            return 0;
        }
    }
    return 0;
};
