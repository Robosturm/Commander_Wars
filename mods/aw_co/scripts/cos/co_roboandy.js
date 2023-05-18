CO_ROBOANDY.superPowerDamage = 3;
CO_ROBOANDY.superPowerHeal = 3;
CO_ROBOANDY.powerDamage = 1;
CO_ROBOANDY.powerHeal = 1;
CO_ROBOANDY.powerOffBonus = 0;
CO_ROBOANDY.powerDefBonus = 10;
CO_ROBOANDY.d2dOffMissfortuneBonus = 2;
CO_ROBOANDY.d2dOffBonus = 4;
CO_ROBOANDY.d2dCoZoneMissfortuneBonus = 0;
CO_ROBOANDY.d2dCoZoneOffBonus = 0;
CO_ROBOANDY.d2dCoZoneOffBaseBonus = 0;
CO_ROBOANDY.d2dCoZoneDefBonus = 0;

CO_ROBOANDY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_ROBOANDY.activateSuperpower = function(co, powerMode, map)
{
    CO_ROBOANDY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ROBOANDY.getSuperPowerDescription = function()
{
    return CO_ROBOANDY.getPowerDescription();
};
CO_ROBOANDY.getSuperPowerName = function()
{
    return CO_ROBOANDY.getPowerName();
};
