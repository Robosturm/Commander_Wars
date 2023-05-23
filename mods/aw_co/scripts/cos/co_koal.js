CO_KOAL.superPowerMovementBonus = 2;
CO_KOAL.superPowerOffBonus = 40;

CO_KOAL.powerMovementBonus = 1;
CO_KOAL.powerOffBonus = 30;
CO_KOAL.powerDefBonus = 10;
CO_KOAL.powerBaseOffBonus = 0;

CO_KOAL.d2dOffBonus = 10;

CO_KOAL.d2dCoZoneOffBonus = 0;
CO_KOAL.d2dCoZoneBaseOffBonus = 0;
CO_KOAL.d2dCoZoneDefBonus = 0;

CO_KOAL.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_KOAL.activateSuperpower = function(co, powerMode, map)
{
    CO_KOAL.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_KOAL.getSuperPowerDescription = function()
{
    return CO_KOAL.getPowerDescription();
};
CO_KOAL.getSuperPowerName = function()
{
    return CO_KOAL.getPowerName();
};
