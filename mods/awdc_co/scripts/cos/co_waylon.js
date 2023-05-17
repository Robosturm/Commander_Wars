CO_WAYLON.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_WAYLON.activateSuperpower = function(co, powerMode, map)
{
	CO_WAYLON.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_WAYLON.getSuperPowerDescription = function()
{
    return CO_WAYLON.getPowerDescription();
};
CO_WAYLON.getSuperPowerName = function()
{
    return CO_WAYLON.getPowerName();
};
