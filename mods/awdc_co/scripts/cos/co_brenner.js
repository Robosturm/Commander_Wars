CO_BRENNER.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_BRENNER.activateSuperpower = function(co, powerMode)
{
	CO_BRENNER.activatePower(co, powerMode);
};
CO_BRENNER.getSuperPowerDescription = function()
{
    return CO_BRENNER.getPowerDescription();
};
CO_BRENNER.getSuperPowerName = function()
{
    return CO_BRENNER.getPowerName();
};