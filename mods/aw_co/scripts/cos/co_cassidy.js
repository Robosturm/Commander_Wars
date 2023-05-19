CO_CASSIDY.superPowerDamage = 1;
CO_CASSIDY.superPowerBonus = 80;

CO_CASSIDY.powerBonus = 60;
CO_CASSIDY.powerOffBonus = 0;
CO_CASSIDY.powerDefBonus = 10;

CO_CASSIDY.d2dOffBonus = 30;

CO_CASSIDY.d2dCoZoneOffBonus = 0;
CO_CASSIDY.d2dCoZoneDefBonus = 0;
CO_CASSIDY.d2dCoZoneBonus = 0;

CO_CASSIDY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_CASSIDY.activateSuperpower = function(co, powerMode, map)
{
    CO_CASSIDY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_CASSIDY.getSuperPowerDescription = function()
{
    return CO_CASSIDY.getPowerDescription();
};
CO_CASSIDY.getSuperPowerName = function()
{
    return CO_CASSIDY.getPowerName();
};
