CO_FLAK.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_FLAK.activateSuperpower = function(co, powerMode)
{
	CO_FLAK.activatePower(co, powerMode);
};
CO_FLAK.getSuperPowerDescription = function()
{
    return CO_FLAK.getPowerDescription();
};
CO_FLAK.getSuperPowerName = function()
{
    return CO_FLAK.getPowerName();
};