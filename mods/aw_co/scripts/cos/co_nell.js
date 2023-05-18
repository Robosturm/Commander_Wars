CO_NELL.superPowerLuckBonus = 100;
CO_NELL.powerLuckBonus = 60;
CO_NELL.powerOffBonus = 0;
CO_NELL.powerDefBonus = 10;
CO_NELL.d2dCoZoneLuckBonus = 0;
CO_NELL.d2dCoZoneOffBonus = 0;
CO_NELL.d2dCoZoneDefBonus = 0;
CO_NELL.d2dLuckBonus = 10;

CO_NELL.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_NELL.activateSuperpower = function(co, powerMode, map)
{
    CO_NELL.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_NELL.getSuperPowerDescription = function()
{
    return CO_NELL.getPowerDescription();
};
CO_NELL.getSuperPowerName = function()
{
    return CO_NELL.getPowerName();
};
