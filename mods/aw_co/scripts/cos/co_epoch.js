CO_EPOCH.superPowerMovementBonus = 1;
CO_EPOCH.superPowerFirerangeBonus = 1;
CO_EPOCH.superPowerLuckBonus = 15;
CO_EPOCH.superPowerOffBonus = 30;
CO_EPOCH.superPowerDefBonus = 30;

CO_EPOCH.powerLuckBonus = 10;
CO_EPOCH.powerOffBonus = 20;
CO_EPOCH.powerDefBonus = 20;

CO_EPOCH.d2dLuckBonus = 10;
CO_EPOCH.d2dOffBonus = 0;
CO_EPOCH.d2dDefBonus = 0;

CO_EPOCH.d2dCoZoneLuckBonus = 0;
CO_EPOCH.d2dCoZoneOffBonus = 0;
CO_EPOCH.d2dCoZoneDefBonus = 0;

CO_EPOCH.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(1);
};
CO_EPOCH.activateSuperpower = function(co, powerMode, map)
{
    CO_EPOCH.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_EPOCH.getSuperPowerDescription = function()
{
    return CO_EPOCH.getPowerDescription();
};
CO_EPOCH.getSuperPowerName = function()
{
    return CO_EPOCH.getPowerName();
};
