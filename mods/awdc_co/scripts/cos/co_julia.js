CO_JULIA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JULIA.activateSuperpower = function(co, powerMode)
{
	CO_JULIA.activatePower(co, powerMode);
};
CO_JULIA.getSuperPowerDescription = function()
{
    return CO_JULIA.getPowerDescription();
};
CO_JULIA.getSuperPowerName = function()
{
    return CO_JULIA.getPowerName();
};