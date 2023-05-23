CO_JAKE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JAKE.activateSuperpower = function(co, powerMode, map)
{
	CO_JAKE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JAKE.getSuperPowerDescription = function()
{
    return CO_JAKE.getPowerDescription();
};
CO_JAKE.getSuperPowerName = function()
{
    return CO_JAKE.getPowerName();
};
