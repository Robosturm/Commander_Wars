CO_MAX.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MAX.activateSuperpower = function(co, powerMode)
{
	CO_MAX.activatePower(co, powerMode);
};
CO_MAX.getSuperPowerDescription = function()
{
    return CO_MAX.getPowerDescription();
};
CO_MAX.getSuperPowerName = function()
{
    return CO_MAX.getPowerName();
};