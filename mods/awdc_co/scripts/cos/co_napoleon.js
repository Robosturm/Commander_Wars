CO_NAPOLEON.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_NAPOLEON.activateSuperpower = function(co, powerMode)
{
	CO_NAPOLEON.activatePower(co, powerMode);
};
CO_NAPOLEON.getSuperPowerDescription = function()
{
    return CO_NAPOLEON.getPowerDescription();
};
CO_NAPOLEON.getSuperPowerName = function()
{
    return CO_NAPOLEON.getPowerName();
};