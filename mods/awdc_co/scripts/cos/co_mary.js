CO_MARY.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MARY.activateSuperpower = function(co, powerMode)
{
	CO_MARY.activatePower(co, powerMode);
};
CO_MARY.getSuperPowerDescription = function()
{
    return CO_MARY.getPowerDescription();
};
CO_MARY.getSuperPowerName = function()
{
    return CO_MARY.getPowerName();
};