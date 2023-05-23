CO_ROBOANDY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ROBOANDY.activateSuperpower = function(co, powerMode, map)
{
	CO_ROBOANDY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ROBOANDY.getSuperPowerDescription = function()
{
    return CO_ROBOANDY.getPowerDescription();
};
CO_ROBOANDY.getSuperPowerName = function()
{
    return CO_ROBOANDY.getPowerName();
};
