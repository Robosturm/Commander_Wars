CO_ROBOSTURM.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ROBOSTURM.activateSuperpower = function(co, powerMode, map)
{
	CO_ROBOSTURM.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ROBOSTURM.getSuperPowerDescription = function()
{
    return CO_ROBOSTURM.getPowerDescription();
};
CO_ROBOSTURM.getSuperPowerName = function()
{
    return CO_ROBOSTURM.getPowerName();
};
