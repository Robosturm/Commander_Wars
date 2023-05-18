CO_SENSEI.superPowerInfOffBonus = 40;
CO_SENSEI.superPowerHeliOffBonus = 80;
CO_SENSEI.powerInfOffBonus = 40;
CO_SENSEI.powerHeliOffBonus = 80;
CO_SENSEI.powerNavalOffBonus = -10;
CO_SENSEI.powerGroundBonus = -10;
CO_SENSEI.powerOffBonus = 0;
CO_SENSEI.powerDefBonus = 10;
CO_SENSEI.powerSpawnHp = 9;
CO_SENSEI.d2dCoZoneGroundBonus = 0;
CO_SENSEI.d2dCoZoneInfOffBonus = 0;
CO_SENSEI.d2dCoZoneHeliOffBonus = 0;
CO_SENSEI.d2dCoZoneNavalOffBonus = 0;
CO_SENSEI.d2dCoZoneOffBonus = 0;
CO_SENSEI.d2dCoZoneDefBonus = 0;
CO_SENSEI.d2dInfOffBonus = 40;
CO_SENSEI.d2dHeliOffBonus = 50;
CO_SENSEI.d2dNavalOffBonus = -10;
CO_SENSEI.d2dGroundBonus = -10;
CO_SENSEI.d2dTransporterMovementBonus =1;

CO_SENSEI.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_SENSEI.activateSuperpower = function(co, powerMode, map)
{
    CO_SENSEI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SENSEI.getSuperPowerDescription = function()
{
    return CO_SENSEI.getPowerDescription();
};
CO_SENSEI.getSuperPowerName = function()
{
    return CO_SENSEI.getPowerName();
};
