CO_SOPHIE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SOPHIE.activateSuperpower = function(co, powerMode, map)
{
	CO_SOPHIE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SOPHIE.getSuperPowerDescription = function()
{
    return CO_SOPHIE.getPowerDescription();
};
CO_SOPHIE.getSuperPowerName = function()
{
    return CO_SOPHIE.getPowerName();
};
