CO_NANA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_NANA.activateSuperpower = function(co, powerMode, map)
{
	CO_NANA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_NANA.getSuperPowerDescription = function()
{
    return CO_NANA.getPowerDescription();
};
CO_NANA.getSuperPowerName = function()
{
    return CO_NANA.getPowerName();
};
