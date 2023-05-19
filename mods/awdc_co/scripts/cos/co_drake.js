CO_DRAKE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_DRAKE.activateSuperpower = function(co, powerMode, map)
{
	CO_DRAKE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_DRAKE.getSuperPowerDescription = function()
{
    return CO_DRAKE.getPowerDescription();
};
CO_DRAKE.getSuperPowerName = function()
{
    return CO_DRAKE.getPowerName();
};
