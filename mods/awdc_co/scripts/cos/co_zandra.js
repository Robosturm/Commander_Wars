CO_ZANDRA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ZANDRA.activateSuperpower = function(co, powerMode, map)
{
	CO_ZANDRA.activatePower(co, powerMode, map);
};
CO_ZANDRA.getSuperPowerDescription = function()
{
    return CO_ZANDRA.getPowerDescription();
};
CO_ZANDRA.getSuperPowerName = function()
{
    return CO_ZANDRA.getPowerName();
};
