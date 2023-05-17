CO_WAYLON.superPowerOffBonus = 50;
CO_WAYLON.superPowerDefBonus = 270;
CO_WAYLON.powerBaseOffBonus = 0;
CO_WAYLON.powerOffBonus = 10;
CO_WAYLON.powerDefBonus = 200;
CO_WAYLON.powerDefBaseBonus = 10;
CO_WAYLON.d2DOffBonus = 0;
CO_WAYLON.d2DDefBonus = 30;
CO_WAYLON.d2DCoZoneDefBonus = 0;
CO_WAYLON.d2DCoZoneDefBaseBonus = 0;
CO_WAYLON.d2DCoZoneOffBonus = 0;
CO_WAYLON.d2DCoZoneOffBaseBonus = 0;

CO_WAYLON.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_WAYLON.activateSuperpower = function(co, powerMode, map)
{
    CO_WAYLON.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_WAYLON.getSuperPowerDescription = function()
{
    return CO_WAYLON.getPowerDescription();
};
CO_WAYLON.getSuperPowerName = function()
{
    return CO_WAYLON.getPowerName();
};
