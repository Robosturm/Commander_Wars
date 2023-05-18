CO_MEIYO.powerSoldier = 10;
CO_MEIYO.powerExperienced = 20;
CO_MEIYO.powerVeteran = 30;
CO_MEIYO.powerElite = 50;

CO_MEIYO.d2dSoldier = -5;
CO_MEIYO.d2dExperienced = 10;
CO_MEIYO.d2dVeteran = 15;
CO_MEIYO.d2dElite = 20;

CO_MEIYO.d2dCoZoneSoldier = 0;
CO_MEIYO.d2dCoZoneExperienced = 0;
CO_MEIYO.d2dCoZoneVeteran = 0;
CO_MEIYO.d2dCoZoneElite = 0;

CO_MEIYO.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_MEIYO.activateSuperpower = function(co, powerMode, map)
{
    CO_MEIYO.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MEIYO.getSuperPowerDescription = function()
{
    return CO_MEIYO.getPowerDescription();
};
CO_MEIYO.getSuperPowerName = function()
{
    return CO_MEIYO.getPowerName();
};
