CO_PETER.superPowerDamage = 2;
CO_PETER.superPowerOffBonus = 60
CO_PETER.powerBaseOffBonus = 0;
CO_PETER.powerOffBonus = 40;
CO_PETER.powerDefBonus = 10;
CO_PETER.d2dCoZoneOffBonus = 0;
CO_PETER.d2dCoZoneBaseOffBonus = 0;
CO_PETER.d2dCoZoneDefBonus = 0;
CO_PETER.d2dOffBonus = 20;
CO_PETER.d2dTerrainMalus = 5;

CO_PETER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_PETER.activateSuperpower = function(co, powerMode, map)
{
    CO_PETER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_PETER.getSuperPowerDescription = function()
{
    return CO_PETER.getPowerDescription();
};
CO_PETER.getSuperPowerName = function()
{
    return CO_PETER.getPowerName();
};
