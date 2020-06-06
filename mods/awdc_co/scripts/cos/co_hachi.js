CO_HACHI.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_HACHI.activateSuperpower = function(co, powerMode)
{
	CO_HACHI.activatePower(co, powerMode);
};
CO_HACHI.getSuperPowerDescription = function()
{
    return CO_HACHI.getPowerDescription();
};
CO_HACHI.getSuperPowerName = function()
{
    return CO_HACHI.getPowerName();
};