CO_TASHA.superPowerOffBonus = 60;
CO_TASHA.superPowerDefBonus = 30;
CO_TASHA.superPowerMovementBonus = 2;
CO_TASHA.powerOffBonus = 40;
CO_TASHA.powerBaseOffBonus = 0;
CO_TASHA.powerDefBonus = 30;
CO_TASHA.powerMovementBonus = 1;
CO_TASHA.powerBaseDefBonus = 10;
CO_TASHA.d2dOffBonus = 15;
CO_TASHA.d2dDeffBonus = 0;
CO_TASHA.d2dCoZoneOffBonus = 0;
CO_TASHA.d2dCoZoneDeffBonus = 0;
CO_TASHA.d2dCoZoneBaseOffBonus = 0;
CO_TASHA.d2dCoZoneBaseDeffBonus = 0;

CO_TASHA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_TASHA.activateSuperpower = function(co, powerMode, map)
{
    CO_TASHA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_TASHA.getSuperPowerDescription = function()
{
    return CO_TASHA.getPowerDescription();
};
CO_TASHA.getSuperPowerName = function()
{
    return CO_TASHA.getPowerName();
};
