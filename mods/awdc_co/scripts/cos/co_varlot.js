CO_VARLOT.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_VARLOT.activateSuperpower = function(co, powerMode, map)
{
	CO_VARLOT.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_VARLOT.getSuperPowerDescription = function()
{
    return CO_VARLOT.getPowerDescription();
};
CO_VARLOT.getSuperPowerName = function()
{
    return CO_VARLOT.getPowerName();
};
