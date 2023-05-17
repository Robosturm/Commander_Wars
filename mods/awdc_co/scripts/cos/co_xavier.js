CO_XAVIER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_XAVIER.activateSuperpower = function(co, powerMode, map)
{
	CO_XAVIER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_XAVIER.getSuperPowerDescription = function()
{
    return CO_XAVIER.getPowerDescription();
};
CO_XAVIER.getSuperPowerName = function()
{
    return CO_XAVIER.getPowerName();
};
