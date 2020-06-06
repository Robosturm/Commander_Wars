CO_SAMI.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SAMI.activateSuperpower = function(co, powerMode)
{
	CO_SAMI.activatePower(co, powerMode);
};
CO_SAMI.getSuperPowerDescription = function()
{
    return CO_SAMI.getPowerDescription();
};
CO_SAMI.getSuperPowerName = function()
{
    return CO_SAMI.getPowerName();
};