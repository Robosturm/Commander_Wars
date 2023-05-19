CO_CASSIDY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_CASSIDY.activateSuperpower = function(co, powerMode, map)
{
	CO_CASSIDY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_CASSIDY.getSuperPowerDescription = function()
{
    return CO_CASSIDY.getPowerDescription();
};
CO_CASSIDY.getSuperPowerName = function()
{
    return CO_CASSIDY.getPowerName();
};
