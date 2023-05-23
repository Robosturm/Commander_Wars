CO_SABAKI.superPowerHeal = 1;
CO_SABAKI.powerHeal = 0.5;
CO_SABAKI.powerDefBonus = 10;
CO_SABAKI.powerOffBonus = 0;
CO_SABAKI.d2dHeal = 0.15;
CO_SABAKI.d2dCoZoneHeal = 0.0;
CO_SABAKI.d2dCoZoneDefBonus = 0;
CO_SABAKI.d2dCoZoneOffBonus = 0;

CO_SABAKI.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_SABAKI.activateSuperpower = function(co, powerMode, map)
{
    CO_SABAKI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SABAKI.getSuperPowerDescription = function()
{
    return CO_SABAKI.getPowerDescription();
};
CO_SABAKI.getSuperPowerName = function()
{
    return CO_SABAKI.getPowerName();
};
