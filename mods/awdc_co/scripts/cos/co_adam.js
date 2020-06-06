CO_ADAM.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ADAM.activateSuperpower = function(co, powerMode)
{
	CO_ADAM.activatePower(co, powerMode);
};
CO_ADAM.getSuperPowerDescription = function()
{
    return CO_ADAM.getPowerDescription();
};
CO_ADAM.getSuperPowerName = function()
{
    return CO_ADAM.getPowerName();
};
