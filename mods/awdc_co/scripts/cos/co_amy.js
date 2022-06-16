CO_AMY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_AMY.activateSuperpower = function(co, powerMode, map)
{
	CO_AMY.activatePower(co, powerMode, map);
};
CO_AMY.getSuperPowerDescription = function()
{
    return CO_AMY.getPowerDescription();
};
CO_AMY.getSuperPowerName = function()
{
    return CO_AMY.getPowerName();
};