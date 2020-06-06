CO_JUGGER.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JUGGER.activateSuperpower = function(co, powerMode)
{
	CO_JUGGER.activatePower(co, powerMode);
};
CO_JUGGER.getSuperPowerDescription = function()
{
    return CO_JUGGER.getPowerDescription();
};
CO_JUGGER.getSuperPowerName = function()
{
    return CO_JUGGER.getPowerName();
};