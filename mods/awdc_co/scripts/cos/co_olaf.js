CO_OLAF.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_OLAF.activateSuperpower = function(co, powerMode, map)
{
	CO_OLAF.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_OLAF.getSuperPowerDescription = function()
{
    return CO_OLAF.getPowerDescription();
};
CO_OLAF.getSuperPowerName = function()
{
    return CO_OLAF.getPowerName();
};
