CO_ROBOSTURM.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ROBOSTURM.activateSuperpower = function(co, powerMode)
{
	CO_ROBOSTURM.activatePower(co, powerMode);
};
CO_ROBOSTURM.getSuperPowerDescription = function()
{
    return CO_ROBOSTURM.getPowerDescription();
};
CO_ROBOSTURM.getSuperPowerName = function()
{
    return CO_ROBOSTURM.getPowerName();
};