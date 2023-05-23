CO_HACHI.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_HACHI.activateSuperpower = function(co, powerMode, map)
{
	CO_HACHI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_HACHI.getSuperPowerDescription = function()
{
    return CO_HACHI.getPowerDescription();
};
CO_HACHI.getSuperPowerName = function()
{
    return CO_HACHI.getPowerName();
};
