CO_STURM.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_STURM.activateSuperpower = function(co, powerMode)
{
	CO_STURM.activatePower(co, powerMode);
};
CO_STURM.getSuperPowerDescription = function()
{
    return CO_STURM.getPowerDescription();
};
CO_STURM.getSuperPowerName = function()
{
    return CO_STURM.getPowerName();
};