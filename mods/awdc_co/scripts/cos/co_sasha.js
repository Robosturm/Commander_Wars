CO_SASHA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SASHA.activateSuperpower = function(co, powerMode)
{
	CO_SASHA.activatePower(co, powerMode);
};
CO_SASHA.getSuperPowerDescription = function()
{
    return CO_SASHA.getPowerDescription();
};
CO_SASHA.getSuperPowerName = function()
{
    return CO_SASHA.getPowerName();
};