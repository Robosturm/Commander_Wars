CO_TABITHA.superPowerDamage = 8;
CO_TABITHA.powerDamage = 4;
CO_TABITHA.superPowerOffBonus = 20;
CO_TABITHA.superPowerDefBonus = 30;
CO_TABITHA.powerOffBonus = 20;
CO_TABITHA.powerDefBonus = 30;
CO_TABITHA.d2dCoZoneOffBonus = 0;
CO_TABITHA.d2dCoZoneDefBonus = 0;
CO_TABITHA.d2dOffBonus = 10;
CO_TABITHA.d2dDefBonus = 10;

CO_TABITHA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_TABITHA.activateSuperpower = function(co, powerMode, map)
{
    CO_TABITHA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_TABITHA.getSuperPowerDescription = function()
{
    return CO_TABITHA.getPowerDescription();
};
CO_TABITHA.getSuperPowerName = function()
{
    return CO_TABITHA.getPowerName();
};
