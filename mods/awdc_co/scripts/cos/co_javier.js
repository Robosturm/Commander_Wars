CO_JAVIER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JAVIER.activateSuperpower = function(co, powerMode, map)
{
	CO_JAVIER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JAVIER.getSuperPowerDescription = function()
{
    return CO_JAVIER.getPowerDescription();
};
CO_JAVIER.getSuperPowerName = function()
{
    return CO_JAVIER.getPowerName();
};
