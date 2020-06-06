CO_PENNY.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_PENNY.activateSuperpower = function(co, powerMode)
{
	CO_PENNY.activatePower(co, powerMode);
};
CO_PENNY.getSuperPowerDescription = function()
{
    return CO_PENNY.getPowerDescription();
};
CO_PENNY.getSuperPowerName = function()
{
    return CO_PENNY.getPowerName();
};