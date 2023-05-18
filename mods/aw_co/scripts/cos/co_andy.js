CO_ANDY.superPowerHeal = 5;
CO_ANDY.superPowerOffBonus = 20;
CO_ANDY.superPowerMovementBonus = 1;

CO_ANDY.powerDefBonus = 10;
CO_ANDY.powerOffBonus = 0;
CO_ANDY.powerHeal = 2;

CO_ANDY.d2dCoZoneOffBonus = 0;
CO_ANDY.d2dCoZoneDefBonus = 0;

CO_ANDY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_ANDY.activateSuperpower = function(co, powerMode, map)
{
    CO_ANDY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ANDY.getSuperPowerDescription = function()
{
    return CO_ANDY.getPowerDescription();
};
CO_ANDY.getSuperPowerName = function()
{
    return CO_ANDY.getPowerName();
};
