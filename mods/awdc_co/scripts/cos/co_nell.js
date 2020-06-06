CO_NELL.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_NELL.activateSuperpower = function(co, powerMode)
{
	CO_NELL.activatePower(co, powerMode);
};
CO_NELL.getSuperPowerDescription = function()
{
    return CO_NELL.getPowerDescription();
};
CO_NELL.getSuperPowerName = function()
{
    return CO_NELL.getPowerName();
};