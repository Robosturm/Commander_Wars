CO_KOAL.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_KOAL.activateSuperpower = function(co, powerMode)
{
	CO_KOAL.activatePower(co, powerMode);
};
CO_KOAL.getSuperPowerDescription = function()
{
    return CO_KOAL.getPowerDescription();
};
CO_KOAL.getSuperPowerName = function()
{
    return CO_KOAL.getPowerName();
};