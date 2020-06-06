CO_KINDLE.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_KINDLE.activateSuperpower = function(co, powerMode)
{
	CO_KINDLE.activatePower(co, powerMode);
};
CO_KINDLE.getSuperPowerDescription = function()
{
    return CO_KINDLE.getPowerDescription();
};
CO_KINDLE.getSuperPowerName = function()
{
    return CO_KINDLE.getPowerName();
};