CO_SABAKI.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SABAKI.activateSuperpower = function(co, powerMode)
{
	CO_SABAKI.activatePower(co, powerMode);
};
CO_SABAKI.getSuperPowerDescription = function()
{
    return CO_SABAKI.getPowerDescription();
};
CO_SABAKI.getSuperPowerName = function()
{
    return CO_SABAKI.getPowerName();
};