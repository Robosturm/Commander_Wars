CO_MARY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MARY.activateSuperpower = function(co, powerMode, map)
{
	CO_MARY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MARY.getSuperPowerDescription = function()
{
    return CO_MARY.getPowerDescription();
};
CO_MARY.getSuperPowerName = function()
{
    return CO_MARY.getPowerName();
};
