CO_KOAL.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_KOAL.activateSuperpower = function(co, powerMode, map)
{
	CO_KOAL.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_KOAL.getSuperPowerDescription = function()
{
    return CO_KOAL.getPowerDescription();
};
CO_KOAL.getSuperPowerName = function()
{
    return CO_KOAL.getPowerName();
};
