CO_AIRA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_AIRA.activateSuperpower = function(co, powerMode)
{
	CO_AIRA.activatePower(co, powerMode);
};
CO_AIRA.getSuperPowerDescription = function()
{
    return CO_AIRA.getPowerDescription();
};
CO_AIRA.getSuperPowerName = function()
{
    return CO_AIRA.getPowerName();
};