CO_JULIA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JULIA.activateSuperpower = function(co, powerMode, map)
{
	CO_JULIA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JULIA.getSuperPowerDescription = function()
{
    return CO_JULIA.getPowerDescription();
};
CO_JULIA.getSuperPowerName = function()
{
    return CO_JULIA.getPowerName();
};
