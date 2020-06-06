CO_ANDY.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ANDY.activateSuperpower = function(co, powerMode)
{
	CO_ANDY.activatePower(co, powerMode);
};
CO_ANDY.getSuperPowerDescription = function()
{
    return CO_ANDY.getPowerDescription();
};
CO_ANDY.getSuperPowerName = function()
{
    return CO_ANDY.getPowerName();
};