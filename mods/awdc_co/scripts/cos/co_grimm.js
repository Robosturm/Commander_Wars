CO_GRIMM.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GRIMM.activateSuperpower = function(co, powerMode)
{
	CO_GRIMM.activatePower(co, powerMode);
};
CO_GRIMM.getSuperPowerDescription = function()
{
    return CO_GRIMM.getPowerDescription();
};
CO_GRIMM.getSuperPowerName = function()
{
    return CO_GRIMM.getPowerName();
};