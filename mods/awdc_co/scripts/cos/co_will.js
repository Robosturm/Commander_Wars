CO_WILL.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_WILL.activateSuperpower = function(co, powerMode)
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
CO_WILL.getMovementpointModifier = function(co, unit, posX, posY)
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