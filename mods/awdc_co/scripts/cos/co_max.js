CO_MAX.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MAX.activateSuperpower = function(co, powerMode, map)
{
	CO_MAX.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MAX.getSuperPowerDescription = function()
{
    return CO_MAX.getPowerDescription();
};
CO_MAX.getSuperPowerName = function()
{
    return CO_MAX.getPowerName();
};
