CO_JUGGER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JUGGER.activateSuperpower = function(co, powerMode, map)
{
	CO_JUGGER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JUGGER.getSuperPowerDescription = function()
{
    return CO_JUGGER.getPowerDescription();
};
CO_JUGGER.getSuperPowerName = function()
{
    return CO_JUGGER.getPowerName();
};
