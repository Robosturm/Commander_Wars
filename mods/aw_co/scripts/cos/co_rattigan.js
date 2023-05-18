CO_RATTIGAN.superPowerDamage = 1;
CO_RATTIGAN.superPowerOffBonus = 40;
CO_RATTIGAN.superPowerDefBonus = 30;
CO_RATTIGAN.powerOffBonus = 20;
CO_RATTIGAN.powerBaseOffBonus = 0;
CO_RATTIGAN.powerBaseDefBonus = 10;
CO_RATTIGAN.powerMultiplier = 5;
CO_RATTIGAN.powerMovementPoints = 1;
CO_RATTIGAN.d2dOffBonus = 25;
CO_RATTIGAN.d2dOffMalus = -10;
CO_RATTIGAN.d2dDefBonus = 0;
CO_RATTIGAN.d2dCoZoneDefBonus = 0;
CO_RATTIGAN.d2dCoZoneOffBonus = 0;
CO_RATTIGAN.d2dCoZoneOffMalus = 0;

CO_RATTIGAN.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_RATTIGAN.activateSuperpower = function(co, powerMode, map)
{
    CO_RATTIGAN.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_RATTIGAN.getSuperPowerDescription = function()
{
    return CO_RATTIGAN.getPowerDescription();
};
CO_RATTIGAN.getSuperPowerName = function()
{
    return CO_RATTIGAN.getPowerName();
};
