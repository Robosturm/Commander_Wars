CO_SENSEI.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SENSEI.activateSuperpower = function(co, powerMode)
{
	CO_SENSEI.activatePower(co, powerMode);
};
CO_SENSEI.getSuperPowerDescription = function()
{
    return CO_SENSEI.getPowerDescription();
};
CO_SENSEI.getSuperPowerName = function()
{
    return CO_SENSEI.getPowerName();
};