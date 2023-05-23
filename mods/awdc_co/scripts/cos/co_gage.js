CO_GAGE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GAGE.activateSuperpower = function(co, powerMode, map)
{
	CO_GAGE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GAGE.getSuperPowerDescription = function()
{
    return CO_GAGE.getPowerDescription();
};
CO_GAGE.getSuperPowerName = function()
{
    return CO_GAGE.getPowerName();
};
