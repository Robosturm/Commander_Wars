CO_ALEXIS.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ALEXIS.activateSuperpower = function(co, powerMode)
{
	CO_ALEXIS.activatePower(co, powerMode);
};
CO_ALEXIS.getSuperPowerDescription = function()
{
    return CO_ALEXIS.getPowerDescription();
};
CO_ALEXIS.getSuperPowerName = function()
{
    return CO_ALEXIS.getPowerName();
};