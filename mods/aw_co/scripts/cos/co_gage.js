CO_GAGE.superPowerFirerangeBonus = 3;
CO_GAGE.superPowerDefBonus = 30;
CO_GAGE.superPowerOffBonus = 50;

CO_GAGE.powerFirerangeBonus = 2;
CO_GAGE.powerDefBonus = 20;
CO_GAGE.powerBaseDefBonus = 10;
CO_GAGE.powerOffBonus = 30;
CO_GAGE.powerBaseOffBonus = 0;

CO_GAGE.d2dOffBonus = 10;
CO_GAGE.d2dDefBonus = 5;

CO_GAGE.d2dCoZoneDefBonus = 0;
CO_GAGE.d2dCoZoneBaseDefBonus = 0;
CO_GAGE.d2dCoZoneOffBonus = 0;
CO_GAGE.d2dCoZoneBaseOffBonus = 0;

CO_GAGE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_GAGE.activateSuperpower = function(co, powerMode, map)
{
    CO_GAGE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GAGE.getSuperPowerDescription = function()
{
    return CO_GAGE.getPowerDescription();
};
CO_GAGE.getSuperPowerName = function()
{
    return CO_GAGE.getPowerName();
};
