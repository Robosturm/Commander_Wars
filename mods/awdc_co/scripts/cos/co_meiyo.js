CO_MEIYO.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MEIYO.activateSuperpower = function(co, powerMode, map)
{
	CO_MEIYO.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MEIYO.getSuperPowerDescription = function()
{
    return CO_MEIYO.getPowerDescription();
};
CO_MEIYO.getSuperPowerName = function()
{
    return CO_MEIYO.getPowerName();
};
