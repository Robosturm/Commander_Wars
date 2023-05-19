CO_JESS.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JESS.activateSuperpower = function(co, powerMode, map)
{
	CO_JESS.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JESS.getSuperPowerDescription = function()
{
    return CO_JESS.getPowerDescription();
};
CO_JESS.getSuperPowerName = function()
{
    return CO_JESS.getPowerName();
};
