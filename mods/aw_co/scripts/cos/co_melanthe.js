CO_MELANTHE.superPowerHeal = 4;
CO_MELANTHE.superPowerDamage = 2;

CO_MELANTHE.powerOffBonus = 20;
CO_MELANTHE.powerBaseOffBonus = 0;
CO_MELANTHE.powerDefBonus = 10;
CO_MELANTHE.powerTerrainBonus = 2;
CO_MELANTHE.powerHeal = 3;
CO_MELANTHE.powerDamage = 0;

CO_MELANTHE.d2dOffBonus = 20;
CO_MELANTHE.d2dTerrainBonus = 1;
CO_MELANTHE.d2dRepairBonus = -1;

CO_MELANTHE.d2dCoZoneOffBonus = 0;
CO_MELANTHE.d2dCoZoneTerrainBonus = 0;
CO_MELANTHE.d2dCoZoneDefBonus = 0;
CO_MELANTHE.d2dCoZoneBaseOffBonus = 0;

CO_MELANTHE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_MELANTHE.activateSuperpower = function(co, powerMode, map)
{
    CO_MELANTHE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MELANTHE.getSuperPowerDescription = function()
{
    return CO_MELANTHE.getPowerDescription();
};
CO_MELANTHE.getSuperPowerName = function()
{
    return CO_MELANTHE.getPowerName();
};
