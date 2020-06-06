CO_DRAKE.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_DRAKE.activateSuperpower = function(co, powerMode)
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