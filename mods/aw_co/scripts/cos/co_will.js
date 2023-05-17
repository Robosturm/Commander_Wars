CO_WILL.superPowerOffBonus = 40;
CO_WILL.superPowerMovementBonus = 2;
CO_WILL.powerOffBaseBonus = 10;
CO_WILL.powerOffBonus = 20;
CO_WILL.powerDefBonus = 10;
CO_WILL.powerMovementBonus = 1;
CO_WILL.d2dOffBonus = 10;
CO_WILL.d2dCoZoneOffBonus = 0;
CO_WILL.d2dCoZoneBaseBonus = 0;
CO_WILL.d2dCoZoneDefBonus = 0;

CO_WILL.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_WILL.activateSuperpower = function(co, powerMode, map)
{
    CO_WILL.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_WILL.getSuperPowerDescription = function()
{
    return CO_WILL.getPowerDescription();
};
CO_WILL.getSuperPowerName = function()
{
    return CO_WILL.getPowerName();
};
