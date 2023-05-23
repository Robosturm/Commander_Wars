CO_SONJA.superPowerEnemyTerrainDefenseModifier = 0;
CO_SONJA.powerOffBonus = 0;
CO_SONJA.powerDefBonus = 10;
CO_SONJA.powerVisionBonus = 2;
CO_SONJA.powerEnemyTerrainDefenseModifier = 0;
CO_SONJA.d2dCoZoneOffBonus = 0;
CO_SONJA.d2dCoZoneDefBonus = 0;
CO_SONJA.d2dVisionBonus = 1;
CO_SONJA.d2dEnemyTerrainDefenseModifier = 0;
CO_SONJA.d2dBonusMisfortune = 15;
CO_SONJA.d2dCounterAttackBonus = 0;

CO_SONJA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_SONJA.activateSuperpower = function(co, powerMode, map)
{
    CO_SONJA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SONJA.getSuperPowerDescription = function()
{
    return CO_SONJA.getPowerDescription();
};
CO_SONJA.getSuperPowerName = function()
{
    return CO_SONJA.getPowerName();
};
