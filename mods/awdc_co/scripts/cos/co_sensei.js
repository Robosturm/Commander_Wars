CO_SENSEI.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SENSEI.activateSuperpower = function(co, powerMode, map)
{
	CO_SENSEI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SENSEI.getSuperPowerDescription = function()
{
    return CO_SENSEI.getPowerDescription();
};
CO_SENSEI.getSuperPowerName = function()
{
    return CO_SENSEI.getPowerName();
};
