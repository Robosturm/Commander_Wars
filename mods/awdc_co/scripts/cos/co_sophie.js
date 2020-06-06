CO_SOPHIE.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SOPHIE.activateSuperpower = function(co, powerMode)
{
	CO_SOPHIE.activatePower(co, powerMode);
};
CO_SOPHIE.getSuperPowerDescription = function()
{
    return CO_SOPHIE.getPowerDescription();
};
CO_SOPHIE.getSuperPowerName = function()
{
    return CO_SOPHIE.getPowerName();
};