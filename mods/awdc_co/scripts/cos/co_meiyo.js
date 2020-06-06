CO_MEIYO.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MEIYO.activateSuperpower = function(co, powerMode)
{
	CO_MEIYO.activatePower(co, powerMode);
};
CO_MEIYO.getSuperPowerDescription = function()
{
    return CO_MEIYO.getPowerDescription();
};
CO_MEIYO.getSuperPowerName = function()
{
    return CO_MEIYO.getPowerName();
};