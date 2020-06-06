CO_SONJA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SONJA.activateSuperpower = function(co, powerMode)
{
	CO_SONJA.activatePower(co, powerMode);
};
CO_SONJA.getSuperPowerDescription = function()
{
    return CO_SONJA.getPowerDescription();
};
CO_SONJA.getSuperPowerName = function()
{
    return CO_SONJA.getPowerName();
};