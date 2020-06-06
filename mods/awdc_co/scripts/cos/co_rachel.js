CO_RACHEL.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_RACHEL.activateSuperpower = function(co, powerMode)
{
	CO_RACHEL.activatePower(co, powerMode);
};
CO_RACHEL.getSuperPowerDescription = function()
{
    return CO_RACHEL.getPowerDescription();
};
CO_RACHEL.getSuperPowerName = function()
{
    return CO_RACHEL.getPowerName();
};