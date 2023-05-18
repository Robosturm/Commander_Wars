CO_RACHEL.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_RACHEL.activateSuperpower = function(co, powerMode, map)
{
	CO_RACHEL.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_RACHEL.getSuperPowerDescription = function()
{
    return CO_RACHEL.getPowerDescription();
};
CO_RACHEL.getSuperPowerName = function()
{
    return CO_RACHEL.getPowerName();
};
