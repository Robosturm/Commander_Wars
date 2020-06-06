CO_SMITAN.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SMITAN.activateSuperpower = function(co, powerMode)
{
	CO_SMITAN.activatePower(co, powerMode);
};
CO_SMITAN.getSuperPowerDescription = function()
{
    return CO_SMITAN.getPowerDescription();
};
CO_SMITAN.getSuperPowerName = function()
{
    return CO_SMITAN.getPowerName();
};