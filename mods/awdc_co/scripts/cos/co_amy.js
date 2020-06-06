CO_AMY.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_AMY.activateSuperpower = function(co, powerMode)
{
	CO_AMY.activatePower(co, powerMode);
};
CO_AMY.getSuperPowerDescription = function()
{
    return CO_AMY.getPowerDescription();
};
CO_AMY.getSuperPowerName = function()
{
    return CO_AMY.getPowerName();
};