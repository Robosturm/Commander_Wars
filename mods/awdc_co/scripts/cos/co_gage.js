CO_GAGE.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GAGE.activateSuperpower = function(co, powerMode)
{
	CO_GAGE.activatePower(co, powerMode);
};
CO_GAGE.getSuperPowerDescription = function()
{
    return CO_GAGE.getPowerDescription();
};
CO_GAGE.getSuperPowerName = function()
{
    return CO_GAGE.getPowerName();
};