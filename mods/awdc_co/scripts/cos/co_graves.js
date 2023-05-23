CO_GRAVES.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GRAVES.activateSuperpower = function(co, powerMode, map)
{
	CO_GRAVES.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GRAVES.getSuperPowerDescription = function()
{
    return CO_GRAVES.getPowerDescription();
};
CO_GRAVES.getSuperPowerName = function()
{
    return CO_GRAVES.getPowerName();
};
