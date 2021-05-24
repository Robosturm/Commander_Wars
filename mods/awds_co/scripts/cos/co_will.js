CO_WILL.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        var seaAirUnit = (attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
            {
                return 50;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
            {
                return 30;
            }
            return 10;
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
                                       defender, defPosX, defPosY, isDefender, action)
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

CO_WILL.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var seaAirUnit = (attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (!seaAirUnit)
            {
                return 2;
            }
            return 0;
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
