CO_LASH.superPowerTerrainDefenseModifier = 1;
CO_LASH.powerOffBonus = 0;
CO_LASH.powerDefBonus = 10;

CO_LASH.d2dTerrainBonus = 10;
CO_LASH.d2dCoZoneTerrainBonus = 0;
CO_LASH.d2dCoZoneOffBonus = 0;
CO_LASH.d2dCoZoneDefBonus = 0;

CO_LASH.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_LASH.activateSuperpower = function(co, powerMode, map)
{
    CO_LASH.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_LASH.getSuperPowerDescription = function()
{
    return CO_LASH.getPowerDescription();
};
CO_LASH.getSuperPowerName = function()
{
    return CO_LASH.getPowerName();
};
