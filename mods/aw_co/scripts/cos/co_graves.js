CO_GRAVES.superPowerDamage = 2;
CO_GRAVES.superPowerStunLevel = 4;

CO_GRAVES.powerDamage = 1;
CO_GRAVES.powerStunLevel = 3;
CO_GRAVES.powerOffBonus = 0;
CO_GRAVES.powerDefBonus = 10;

CO_GRAVES.d2dStunLevel = 2;

CO_GRAVES.d2dCoZoneOffBonus = 0;
CO_GRAVES.d2dCoZoneDefBonus = 0;

CO_GRAVES.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_GRAVES.activateSuperpower = function(co, powerMode, map)
{
    CO_GRAVES.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GRAVES.getSuperPowerDescription = function()
{
    return CO_GRAVES.getPowerDescription();
};
CO_GRAVES.getSuperPowerName = function()
{
    return CO_GRAVES.getPowerName();
};
