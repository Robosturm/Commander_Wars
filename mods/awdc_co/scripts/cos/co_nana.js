CO_NANA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_NANA.activateSuperpower = function(co, powerMode)
{
	CO_NANA.activatePower(co, powerMode);
};
CO_NANA.getSuperPowerDescription = function()
{
    return CO_NANA.getPowerDescription();
};
CO_NANA.getSuperPowerName = function()
{
    return CO_NANA.getPowerName();
};