CO_HAWKE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_HAWKE.activateSuperpower = function(co, powerMode, map)
{
	CO_HAWKE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_HAWKE.getSuperPowerDescription = function()
{
    return CO_HAWKE.getPowerDescription();
};
CO_HAWKE.getSuperPowerName = function()
{
    return CO_HAWKE.getPowerName();
};
