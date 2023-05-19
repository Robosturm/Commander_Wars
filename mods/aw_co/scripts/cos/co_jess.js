CO_JESS.superPowerMovementBonus = 2;
CO_JESS.superPowerGroundBonus = 70;
CO_JESS.superPowerNavalAirBonus = -10;
CO_JESS.superPowerInfBonus = 0;

CO_JESS.powerMovementBonus = 1;
CO_JESS.powerDefBonus = 10;
CO_JESS.powerOffBonus = 0;
CO_JESS.powerGroundBonus = 50;
CO_JESS.powerNavalAirBonus = -10;
CO_JESS.powerInfBonus = 0;

CO_JESS.d2dCoZoneDefBonus = 0;
CO_JESS.d2dCoZoneOffBonus = 0;
CO_JESS.d2dCoZoneGroundBonus = 0;
CO_JESS.d2dCoZoneNavalAirBonus = 0;
CO_JESS.d2dCoZoneInfBonus = 0;

CO_JESS.d2dGroundBonus = 20;
CO_JESS.d2dNavalAirBonus = -10;
CO_JESS.d2dInfBonus = 0;

CO_JESS.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_JESS.activateSuperpower = function(co, powerMode, map)
{
    CO_JESS.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JESS.getSuperPowerDescription = function()
{
    return CO_JESS.getPowerDescription();
};
CO_JESS.getSuperPowerName = function()
{
    return CO_JESS.getPowerName();
};
