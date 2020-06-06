CO_MINA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MINA.activateSuperpower = function(co, powerMode)
{
	CO_MINA.activatePower(co, powerMode);
};
CO_MINA.getSuperPowerDescription = function()
{
    return CO_MINA.getPowerDescription();
};
CO_MINA.getSuperPowerName = function()
{
    return CO_MINA.getPowerName();
};