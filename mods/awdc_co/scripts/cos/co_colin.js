CO_COLIN.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_COLIN.activateSuperpower = function(co, powerMode, map)
{
	CO_COLIN.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_COLIN.getSuperPowerDescription = function()
{
    return CO_COLIN.getPowerDescription();
};
CO_COLIN.getSuperPowerName = function()
{
    return CO_COLIN.getPowerName();
};
