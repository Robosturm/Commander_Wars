CO_RATTIGAN.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_RATTIGAN.activateSuperpower = function(co, powerMode, map)
{
	CO_RATTIGAN.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_RATTIGAN.getSuperPowerDescription = function()
{
    return CO_RATTIGAN.getPowerDescription();
};
CO_RATTIGAN.getSuperPowerName = function()
{
    return CO_RATTIGAN.getPowerName();
};
