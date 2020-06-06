CO_EPOCH.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_EPOCH.activateSuperpower = function(co, powerMode)
{
	CO_EPOCH.activatePower(co, powerMode);
};
CO_EPOCH.getSuperPowerDescription = function()
{
    return CO_EPOCH.getPowerDescription();
};
CO_EPOCH.getSuperPowerName = function()
{
    return CO_EPOCH.getPowerName();
};