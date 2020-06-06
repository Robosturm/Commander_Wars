CO_OZZY.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_OZZY.activateSuperpower = function(co, powerMode)
{
	CO_OZZY.activatePower(co, powerMode);
};
CO_OZZY.getSuperPowerDescription = function()
{
    return CO_OZZY.getPowerDescription();
};
CO_OZZY.getSuperPowerName = function()
{
    return CO_OZZY.getPowerName();
};