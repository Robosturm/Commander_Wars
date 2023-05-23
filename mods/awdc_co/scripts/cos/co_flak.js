CO_FLAK.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_FLAK.activateSuperpower = function(co, powerMode, map)
{
	CO_FLAK.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_FLAK.getSuperPowerDescription = function()
{
    return CO_FLAK.getPowerDescription();
};
CO_FLAK.getSuperPowerName = function()
{
    return CO_FLAK.getPowerName();
};
