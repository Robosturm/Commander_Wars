CO_WILL.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_WILL.activateSuperpower = function(co, powerMode, map)
{
	CO_WILL.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_WILL.getSuperPowerDescription = function()
{
    return CO_WILL.getPowerDescription();
};
CO_WILL.getSuperPowerName = function()
{
    return CO_WILL.getPowerName();
};
