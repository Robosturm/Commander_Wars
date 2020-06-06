CO_SANJURO.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SANJURO.activateSuperpower = function(co, powerMode)
{
	CO_SANJURO.activatePower(co, powerMode);
};
CO_SANJURO.getSuperPowerDescription = function()
{
    return CO_SANJURO.getPowerDescription();
};
CO_SANJURO.getSuperPowerName = function()
{
    return CO_SANJURO.getPowerName();
};