CO_JOEY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JOEY.activateSuperpower = function(co, powerMode, map)
{
	CO_JOEY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JOEY.getSuperPowerDescription = function()
{
    return CO_JOEY.getPowerDescription();
};
CO_JOEY.getSuperPowerName = function()
{
    return CO_JOEY.getPowerName();
};
