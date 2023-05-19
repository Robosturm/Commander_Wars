CO_CAULDER.superPowerHeal = 10;
CO_CAULDER.superPowerBonus = 60;

CO_CAULDER.powerHeal = 5;
CO_CAULDER.powerBonus = 40;

CO_CAULDER.d2dHealing = 2;
CO_CAULDER.d2dOffBonus = 10;
CO_CAULDER.d2dDefBonus = 10;

CO_CAULDER.d2dCoZoneOffBonus = 0;
CO_CAULDER.d2dCoZoneDefBonus = 0;
CO_CAULDER.d2dCoZoneHealing = 0;

CO_CAULDER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(7);
};
CO_CAULDER.activateSuperpower = function(co, powerMode, map)
{
    CO_CAULDER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_CAULDER.getSuperPowerDescription = function()
{
    return CO_CAULDER.getPowerDescription();
};
CO_CAULDER.getSuperPowerName = function()
{
    return CO_CAULDER.getPowerName();
};
