CO_XAVIER.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_XAVIER.activateSuperpower = function(co, powerMode)
{
	CO_XAVIER.activatePower(co, powerMode);
};
CO_XAVIER.getSuperPowerDescription = function()
{
    return CO_XAVIER.getPowerDescription();
};
CO_XAVIER.getSuperPowerName = function()
{
    return CO_XAVIER.getPowerName();
};