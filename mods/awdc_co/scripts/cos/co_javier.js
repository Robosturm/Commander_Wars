CO_JAVIER.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JAVIER.activateSuperpower = function(co, powerMode)
{
	CO_JAVIER.activatePower(co, powerMode);
};
CO_JAVIER.getSuperPowerDescription = function()
{
    return CO_JAVIER.getPowerDescription();
};
CO_JAVIER.getSuperPowerName = function()
{
    return CO_JAVIER.getPowerName();
};