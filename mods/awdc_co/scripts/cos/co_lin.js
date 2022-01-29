CO_LIN.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_LIN.activateSuperpower = function(co, powerMode, map)
{
	CO_LIN.activatePower(co, powerMode);
};
CO_LIN.getSuperPowerDescription = function()
{
    return CO_LIN.getPowerDescription();
};
CO_LIN.getSuperPowerName = function()
{
    return CO_LIN.getPowerName();
};
CO_LIN.getVisionrangeModifier = function(co, unit, posX, posY, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 2;
        default:
            return 0;
    }
};

CO_LIN.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, map)
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
                return 30;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (!seaAirUnit)
                {
                    return 30;
                }
                return 10;
            }
            break;
    }
    return 0;
};

CO_LIN.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isAttacker, action, map)
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
            if (co.inCORange(Qt.point(atkPosX, atkPosY), defender))
            {
                if (!seaAirUnit)
                {
                    return 30;
                }
                return 10;
            }
            break;
    }
    return 0;
};
