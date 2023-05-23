CO_HAWKE.superPowerDamage = 2;
CO_HAWKE.superPowerOffBonus = 10;

CO_HAWKE.powerDamage = 1;
CO_HAWKE.powerOffBonus = 10;
CO_HAWKE.powerDefBonus = 10;

CO_HAWKE.d2dOffBonus = 10;
CO_HAWKE.d2dDefBonus = 0;

CO_HAWKE.d2dCoZoneOffBonus = 0;
CO_HAWKE.d2dCoZoneDefBonus = 0;

CO_HAWKE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_HAWKE.activateSuperpower = function(co, powerMode, map)
{
    CO_HAWKE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_HAWKE.getSuperPowerDescription = function()
{
    return CO_HAWKE.getPowerDescription();
};
CO_HAWKE.getSuperPowerName = function()
{
    return CO_HAWKE.getPowerName();
};

