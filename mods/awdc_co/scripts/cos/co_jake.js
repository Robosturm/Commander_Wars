CO_JAKE.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JAKE.activateSuperpower = function(co, powerMode)
{
	CO_JAKE.activatePower(co, powerMode);
};
CO_JAKE.getSuperPowerDescription = function()
{
    return CO_JAKE.getPowerDescription();
};
CO_JAKE.getSuperPowerName = function()
{
    return CO_JAKE.getPowerName();
};