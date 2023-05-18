CO_SMITAN.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SMITAN.activateSuperpower = function(co, powerMode, map)
{
	CO_SMITAN.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SMITAN.getSuperPowerDescription = function()
{
    return CO_SMITAN.getPowerDescription();
};
CO_SMITAN.getSuperPowerName = function()
{
    return CO_SMITAN.getPowerName();
};
