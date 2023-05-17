CO_ALEXANDER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ALEXANDER.activateSuperpower = function(co, powerMode, map)
{
	CO_ALEXANDER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ALEXANDER.getSuperPowerDescription = function()
{
    return CO_ALEXANDER.getPowerDescription();
};
CO_ALEXANDER.getSuperPowerName = function()
{
    return CO_ALEXANDER.getPowerName();
};
