CO_DAVIS.superPowerMaxLuck = 150;
CO_DAVIS.superPowerLuckPerUnit = 6;
CO_DAVIS.superPowerMaxMissFortune = 100;
CO_DAVIS.superPowerMissFortunePerUnit = 4;

CO_DAVIS.powerMaxLuck = 100;
CO_DAVIS.powerLuckPerUnit = 4;
CO_DAVIS.powerMaxMissFortune = 60;
CO_DAVIS.powerMissFortunePerUnit = 3;
CO_DAVIS.powerDefBonus = 10;
CO_DAVIS.powerOffBonus = 10;

CO_DAVIS.d2dMaxLuck = 0;
CO_DAVIS.d2dLuckPerUnit = 0;
CO_DAVIS.d2dMaxMissFortune = 0;
CO_DAVIS.d2dMissFortunePerUnit = 0;

CO_DAVIS.d2dCoZoneMaxLuck = 50;
CO_DAVIS.d2dCoZoneLuckPerUnit = 2;
CO_DAVIS.d2dCoZoneMaxMissFortune = 40;
CO_DAVIS.d2dCoZoneMissFortunePerUnit = 2;
CO_DAVIS.d2dCoZoneDefBonus = 10;
CO_DAVIS.d2dCoZoneOffBonus = 10;

CO_DAVIS.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_DAVIS.activateSuperpower = function(co, powerMode, map)
{
    CO_DAVIS.activatePower(co, map);
};
CO_DAVIS.getSuperPowerDescription = function()
{
    return CO_DAVIS.getPowerDescription();
};
CO_DAVIS.getSuperPowerName = function()
{
    return CO_DAVIS.getPowerName();
};
