CO_TASHA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_TASHA.activateSuperpower = function(co, powerMode)
{
	CO_TASHA.activatePower(co, powerMode);
};
CO_TASHA.getSuperPowerDescription = function()
{
    return CO_TASHA.getPowerDescription();
};
CO_TASHA.getSuperPowerName = function()
{
    return CO_TASHA.getPowerName();
};
CO_TASHA.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
        co.getPowerMode() === GameEnums.PowerMode_Tagpower ||
		co.getPowerMode() === GameEnums.PowerMode_Power)
    {
        return 2;
    }
    return 0;
};