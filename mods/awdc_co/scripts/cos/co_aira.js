CO_AIRA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_AIRA.activateSuperpower = function(co, powerMode, map)
{
	CO_AIRA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_AIRA.getSuperPowerDescription = function()
{
    return CO_AIRA.getPowerDescription();
};
CO_AIRA.getSuperPowerName = function()
{
    return CO_AIRA.getPowerName();
};
