CO_JESS.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JESS.activateSuperpower = function(co, powerMode)
{
	CO_JESS.activatePower(co, powerMode);
};
CO_JESS.getSuperPowerDescription = function()
{
    return CO_JESS.getPowerDescription();
};
CO_JESS.getSuperPowerName = function()
{
    return CO_JESS.getPowerName();
};