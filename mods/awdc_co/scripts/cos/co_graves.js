CO_GRAVES.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GRAVES.activateSuperpower = function(co, powerMode)
{
	CO_GRAVES.activatePower(co, powerMode);
};
CO_GRAVES.getSuperPowerDescription = function()
{
    return CO_GRAVES.getPowerDescription();
};
CO_GRAVES.getSuperPowerName = function()
{
    return CO_GRAVES.getPowerName();
};