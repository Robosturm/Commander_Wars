CO_MINAMOTO.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MINAMOTO.activateSuperpower = function(co, powerMode, map)
{
	CO_MINAMOTO.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MINAMOTO.getSuperPowerDescription = function()
{
    return CO_MINAMOTO.getPowerDescription();
};
CO_MINAMOTO.getSuperPowerName = function()
{
    return CO_MINAMOTO.getPowerName();
};
