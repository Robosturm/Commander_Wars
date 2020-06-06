CO_EAGLE.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_EAGLE.activateSuperpower = function(co, powerMode)
{
	CO_EAGLE.activatePower(co, powerMode);
};
CO_EAGLE.getSuperPowerDescription = function()
{
    return CO_EAGLE.getPowerDescription();
};
CO_EAGLE.getSuperPowerName = function()
{
    return CO_EAGLE.getPowerName();
};