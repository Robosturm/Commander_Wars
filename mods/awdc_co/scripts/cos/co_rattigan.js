CO_RATTIGAN.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_RATTIGAN.activateSuperpower = function(co, powerMode)
{
	CO_RATTIGAN.activatePower(co, powerMode);
};
CO_RATTIGAN.getSuperPowerDescription = function()
{
    return CO_RATTIGAN.getPowerDescription();
};
CO_RATTIGAN.getSuperPowerName = function()
{
    return CO_RATTIGAN.getPowerName();
};