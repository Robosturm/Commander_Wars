CO_MINAMOTO.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MINAMOTO.activateSuperpower = function(co, powerMode)
{
	CO_MINAMOTO.activatePower(co, powerMode);
};
CO_MINAMOTO.getSuperPowerDescription = function()
{
    return CO_MINAMOTO.getPowerDescription();
};
CO_MINAMOTO.getSuperPowerName = function()
{
    return CO_MINAMOTO.getPowerName();
};