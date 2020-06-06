CO_ALEXANDER.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ALEXANDER.activateSuperpower = function(co, powerMode)
{
	CO_ALEXANDER.activatePower(co, powerMode);
};
CO_ALEXANDER.getSuperPowerDescription = function()
{
    return CO_ALEXANDER.getPowerDescription();
};
CO_ALEXANDER.getSuperPowerName = function()
{
    return CO_ALEXANDER.getPowerName();
};