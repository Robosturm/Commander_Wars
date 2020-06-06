CO_LASH.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_LASH.activateSuperpower = function(co, powerMode)
{
	CO_LASH.activatePower(co, powerMode);
};
CO_LASH.getSuperPowerDescription = function()
{
    return CO_LASH.getPowerDescription();
};
CO_LASH.getSuperPowerName = function()
{
    return CO_LASH.getPowerName();
};