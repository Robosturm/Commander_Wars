CO_CASSIDY.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_CASSIDY.activateSuperpower = function(co, powerMode)
{
	CO_CASSIDY.activatePower(co, powerMode);
};
CO_CASSIDY.getSuperPowerDescription = function()
{
    return CO_CASSIDY.getPowerDescription();
};
CO_CASSIDY.getSuperPowerName = function()
{
    return CO_CASSIDY.getPowerName();
};