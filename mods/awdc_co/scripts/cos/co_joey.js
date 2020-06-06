CO_JOEY.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JOEY.activateSuperpower = function(co, powerMode)
{
	CO_JOEY.activatePower(co, powerMode);
};
CO_JOEY.getSuperPowerDescription = function()
{
    return CO_JOEY.getPowerDescription();
};
CO_JOEY.getSuperPowerName = function()
{
    return CO_JOEY.getPowerName();
};