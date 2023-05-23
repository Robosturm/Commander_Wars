CO_NAPOLEON.superPowerTerrainBonus = 10;
CO_NAPOLEON.superPowerOffBonus = 0;
CO_NAPOLEON.superPowerHpBonus = 4;
CO_NAPOLEON.superPowerIndirectDefBonus = 60;
CO_NAPOLEON.superPowerDefBonus = 40;

CO_NAPOLEON.powerDefReduction = 0.5;
CO_NAPOLEON.powerOffBonus = 0;
CO_NAPOLEON.powerIndirectDefBonus = 40;
CO_NAPOLEON.powerDefBonus = 30;

CO_NAPOLEON.d2dIndirectDefBonus = 30;
CO_NAPOLEON.d2dDefBonus = 20;
CO_NAPOLEON.d2dOffBonus = 0;

CO_NAPOLEON.d2dCoZoneOffBonus = 0;
CO_NAPOLEON.d2dCoZoneIndirectDefBonus = 0;
CO_NAPOLEON.d2dCoZoneDefBonus = 0;

CO_NAPOLEON.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_NAPOLEON.activateSuperpower = function(co, powerMode, map)
{
    CO_NAPOLEON.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_NAPOLEON.getSuperPowerDescription = function()
{
    return CO_NAPOLEON.getPowerDescription();
};
CO_NAPOLEON.getSuperPowerName = function()
{
    return CO_NAPOLEON.getPowerName();
};
