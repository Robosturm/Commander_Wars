CO_LIN.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_LIN.activateSuperpower = function(co, powerMode, map)
{
    CO_LIN.activatePower(co, map);
};
CO_LIN.getSuperPowerDescription = function()
{
    return CO_LIN.getPowerDescription();
};
CO_LIN.getSuperPowerName = function()
{
    return CO_LIN.getPowerName();
};
CO_LIN.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                    defender, defPosX, defPosY, isDefender, action, luckmode, map)
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
                return 20;
            }
            return 0;
        default:
            if (!seaAirUnit)
            {
                return 10;
            }
            break;
        }
    }
    return 0;
};

CO_LIN.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        var seaAirUnit = (defender.getUnitType() === GameEnums.UnitType_Air) ||
                         (defender.getUnitType() === GameEnums.UnitType_Naval);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (!seaAirUnit)
            {
                return 30;
            }
            return 10;
        default:
            return 5;
        }
    }
    return 0;
};

CO_LIN.getVisionrangeModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 1;
        default:
            return 0;
        }
    }
    return 0;
};

CO_LIN.getPerfectVision = function(co, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return true;
        default:
            return false;
        }
    }
};
