CO_VARLOT.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_VARLOT.activateSuperpower = function(co, powerMode)
{
	CO_VARLOT.activatePower(co, powerMode);
};
CO_VARLOT.getSuperPowerDescription = function()
{
    return CO_VARLOT.getPowerDescription();
};
CO_VARLOT.getSuperPowerName = function()
{
    return CO_VARLOT.getPowerName();
};