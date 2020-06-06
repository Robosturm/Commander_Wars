CO_GRIT.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GRIT.activateSuperpower = function(co, powerMode)
{
	CO_GRIT.activatePower(co, powerMode);
};
CO_GRIT.getSuperPowerDescription = function()
{
    return CO_GRIT.getPowerDescription();
};
CO_GRIT.getSuperPowerName = function()
{
    return CO_GRIT.getPowerName();
};