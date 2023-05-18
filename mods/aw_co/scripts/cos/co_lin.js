CO_LIN.superPowerVisionRange = 2;
CO_LIN.superPowerOffBonus = 40;
CO_LIN.superPowerBaseOffBonus = 0;
CO_LIN.superPowerDefBonus = 50;
CO_LIN.superPowerBaseDefBonus = 10;

CO_LIN.powerVisionRange = 1;
CO_LIN.powerOffBonus = 20;
CO_LIN.powerBaseOffBonus = 0;
CO_LIN.powerDefBonus = 30;
CO_LIN.powerBaseDefBonus = 10;

CO_LIN.d2dOffBonus = 10;
CO_LIN.d2dBaseOffBonus = 0;
CO_LIN.d2dDefBonus = 5;
CO_LIN.d2dBaseDefBonus = 0;

CO_LIN.d2dCoZoneOffBonus = 0;
CO_LIN.d2dCoZoneBaseOffBonus = 0;
CO_LIN.d2dCoZoneDefBonus = 0;
CO_LIN.d2dCoZoneBaseDefBonus = 0;

CO_LIN.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_LIN.activateSuperpower = function(co, powerMode, map)
{
    CO_LIN.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_LIN.getSuperPowerDescription = function()
{
    return CO_LIN.getPowerDescription();
};
CO_LIN.getSuperPowerName = function()
{
    return CO_LIN.getPowerName();
};
