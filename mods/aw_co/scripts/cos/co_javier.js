CO_JAVIER.superPowerTowerOffBonus = 20;
CO_JAVIER.superPowerTowerDefBonus = 30;
CO_JAVIER.superPowerIndirectDefBonus = 60;

CO_JAVIER.powerTowerOffBonus = 10;
CO_JAVIER.powerTowerDefBonus = 20;
CO_JAVIER.powerIndirectDefBonus = 40;
CO_JAVIER.powerOffBonus = 0;
CO_JAVIER.powerDefBonus = 10;

CO_JAVIER.d2dCoZoneOffBonus = 0;
CO_JAVIER.d2dCoZoneDefBonus = 0;
CO_JAVIER.d2dCoZoneTowerOffBonus = 0;
CO_JAVIER.d2dCoZoneTowerDefBonus = 0;
CO_JAVIER.d2dCoZoneIndirectDefBonus = 0;

CO_JAVIER.d2dTowerOffBonus = 0;
CO_JAVIER.d2dTowerDefBonus = 10;
CO_JAVIER.d2dIndirectDefBonus = 20;

CO_JAVIER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_JAVIER.activateSuperpower = function(co, powerMode, map)
{
    CO_JAVIER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JAVIER.getSuperPowerDescription = function()
{
    return CO_JAVIER.getPowerDescription();
};
CO_JAVIER.getSuperPowerName = function()
{
    return CO_JAVIER.getPowerName();
};
