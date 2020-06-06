CO_ROBOANDY.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ROBOANDY.activateSuperpower = function(co, powerMode)
{
	CO_ROBOANDY.activatePower(co, powerMode);
};
CO_ROBOANDY.getSuperPowerDescription = function()
{
    return CO_ROBOANDY.getPowerDescription();
};
CO_ROBOANDY.getSuperPowerName = function()
{
    return CO_ROBOANDY.getPowerName();
};