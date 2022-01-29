CO_DRAKE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_DRAKE.activateSuperpower = function(co, powerMode, map)
{
	CO_DRAKE.activatePower(co, powerMode);
};
CO_DRAKE.getSuperPowerDescription = function()
{
    return CO_DRAKE.getPowerDescription();
};
CO_DRAKE.getSuperPowerName = function()
{
    return CO_DRAKE.getPowerName();
};
CO_DRAKE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 40;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Air)
            {
                return -5;
            }
            break;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return 40;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return 0;
                }
                return 10;
            }
            break;
    }
    if (attacker.getUnitType() === GameEnums.UnitType_Naval)
    {
        return 10;
    }
    if (attacker.getUnitType() === GameEnums.UnitType_Air)
    {
        return -10;
    }
    return 0;
};
