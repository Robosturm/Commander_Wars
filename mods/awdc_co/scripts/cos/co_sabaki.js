CO_SABAKI.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SABAKI.activateSuperpower = function(co, powerMode, map)
{
	CO_SABAKI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SABAKI.getSuperPowerDescription = function()
{
    return CO_SABAKI.getPowerDescription();
};
CO_SABAKI.getSuperPowerName = function()
{
    return CO_SABAKI.getPowerName();
};
