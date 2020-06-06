CO_HAWKE.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_HAWKE.activateSuperpower = function(co, powerMode)
{
	CO_HAWKE.activatePower(co, powerMode);
};
CO_HAWKE.getSuperPowerDescription = function()
{
    return CO_HAWKE.getPowerDescription();
};
CO_HAWKE.getSuperPowerName = function()
{
    return CO_HAWKE.getPowerName();
};