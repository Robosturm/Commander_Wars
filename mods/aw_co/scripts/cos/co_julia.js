CO_JULIA.superPowerStunChance = 1;

CO_JULIA.powerStunChance = 0.5;
CO_JULIA.powerOffBonus = 70;
CO_JULIA.powerDefBonus = 10;

CO_JULIA.d2dCoZoneOffBonus = 0;
CO_JULIA.d2dCoZoneDefBonus = 0;

CO_JULIA.d2dOffBonus = 70;

CO_JULIA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_JULIA.activateSuperpower = function(co, powerMode, map)
{
    CO_JULIA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JULIA.getSuperPowerDescription = function()
{
    return CO_JULIA.getPowerDescription();
};
CO_JULIA.getSuperPowerName = function()
{
    return CO_JULIA.getPowerName();
};
