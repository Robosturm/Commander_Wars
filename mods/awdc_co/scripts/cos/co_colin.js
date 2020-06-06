CO_COLIN.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_COLIN.activateSuperpower = function(co, powerMode)
{
	CO_COLIN.activatePower(co, powerMode);
};
CO_COLIN.getSuperPowerDescription = function()
{
    return CO_COLIN.getPowerDescription();
};
CO_COLIN.getSuperPowerName = function()
{
    return CO_COLIN.getPowerName();
};