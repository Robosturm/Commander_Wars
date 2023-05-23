CO_ADAM.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ADAM.activateSuperpower = function(co, powerMode, map)
{
	CO_ADAM.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ADAM.getSuperPowerDescription = function()
{
    return CO_ADAM.getPowerDescription();
};
CO_ADAM.getSuperPowerName = function()
{
    return CO_ADAM.getPowerName();
};
