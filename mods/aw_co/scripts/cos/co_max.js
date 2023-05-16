CO_MAX.superPowerOffBonus = 70;
CO_MAX.superpowerMovementBonus = 2;
CO_MAX.powerOffBonus = 45;
CO_MAX.powerMovementBonus = 1
CO_MAX.powerIndirectOffBonus = 0;
CO_MAX.powerOtherBonus = 0;
CO_MAX.powerDefBonus = 10;
CO_MAX.d2dCoZoneDefBonus = 0;
CO_MAX.d2dCoZoneOffBonus = 50;
CO_MAX.d2dCoZoneIndirectOffBonus = 10;
CO_MAX.d2dCoZoneOtherOffBonus = 0;
CO_MAX.d2dIndirectFirerangeMalus = 1;
CO_MAX.d2dOffBonus = 50;
CO_MAX.d2dIndirectOffBonus = 10;
CO_MAX.d2dOtherOffBonus = 0;

CO_MAX.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_MAX.activateSuperpower = function(co, powerMode, map)
{
    CO_MAX.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MAX.getSuperPowerDescription = function()
{
    return CO_MAX.getPowerDescription();
};
CO_MAX.getSuperPowerName = function()
{
    return CO_MAX.getPowerName();
};
