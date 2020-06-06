CO_CONRAD.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_CONRAD.activateSuperpower = function(co, powerMode)
{
	CO_CONRAD.activatePower(co, powerMode);
};
CO_CONRAD.getSuperPowerDescription = function()
{
    return CO_CONRAD.getPowerDescription();
};
CO_CONRAD.getSuperPowerName = function()
{
    return CO_CONRAD.getPowerName();
};