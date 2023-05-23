CO_STURM.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_STURM.activateSuperpower = function(co, powerMode, map)
{
	CO_STURM.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_STURM.getSuperPowerDescription = function()
{
    return CO_STURM.getPowerDescription();
};
CO_STURM.getSuperPowerName = function()
{
    return CO_STURM.getPowerName();
};
