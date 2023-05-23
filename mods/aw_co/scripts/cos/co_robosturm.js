CO_ROBOSTURM.superPowerDamage = 2;
CO_ROBOSTURM.superPowerMovementPoints = 3;
CO_ROBOSTURM.superPowerDefBonus = 80;
CO_ROBOSTURM.superPowerOffBonus = 0;
CO_ROBOSTURM.powerDamage = 0;
CO_ROBOSTURM.powerMovementPoints = 2;
CO_ROBOSTURM.powerDefBonus = 60;
CO_ROBOSTURM.powerOffBonus = 0;
CO_ROBOSTURM.d2dCoZoneOffBonus = 0;
CO_ROBOSTURM.d2dCoZoneDefBonus = 0;
CO_ROBOSTURM.d2dOffBonus = -20;
CO_ROBOSTURM.d2dDefBonus = 20;
CO_ROBOSTURM.d2dMovementPoints = 1;

CO_ROBOSTURM.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_ROBOSTURM.activateSuperpower = function(co, powerMode, map)
{
    CO_ROBOSTURM.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ROBOSTURM.getSuperPowerDescription = function()
{
    return CO_ROBOSTURM.getPowerDescription();
};
CO_ROBOSTURM.getSuperPowerName = function()
{
    return CO_ROBOSTURM.getPowerName();
};
