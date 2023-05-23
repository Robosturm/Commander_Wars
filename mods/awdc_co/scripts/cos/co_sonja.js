CO_SONJA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SONJA.activateSuperpower = function(co, powerMode, map)
{
	CO_SONJA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SONJA.getSuperPowerDescription = function()
{
    return CO_SONJA.getPowerDescription();
};
CO_SONJA.getSuperPowerName = function()
{
    return CO_SONJA.getPowerName();
};
