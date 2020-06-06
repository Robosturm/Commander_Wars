CO_PETER.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_PETER.activateSuperpower = function(co, powerMode)
{
	CO_PETER.activatePower(co, powerMode);
};
CO_PETER.getSuperPowerDescription = function()
{
    return CO_PETER.getPowerDescription();
};
CO_PETER.getSuperPowerName = function()
{
    return CO_PETER.getPowerName();
};