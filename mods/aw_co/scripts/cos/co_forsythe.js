CO_FORSYTHE.superPowerOffBonus = 30;
CO_FORSYTHE.superPowerDefBonus = 30;

CO_FORSYTHE.powerOffBonus = 20;
CO_FORSYTHE.powerDefBonus = 20;

CO_FORSYTHE.d2dOffBonus = 10;
CO_FORSYTHE.d2dDefBonus = 10;

CO_FORSYTHE.d2dCoZoneOffBonus = 0;
CO_FORSYTHE.d2dCoZoneDefBonus = 0;

CO_FORSYTHE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_FORSYTHE.activateSuperpower = function(co, powerMode, map)
{
    CO_FORSYTHE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_FORSYTHE.getSuperPowerDescription = function()
{
    return CO_FORSYTHE.getPowerDescription();
};
CO_FORSYTHE.getSuperPowerName = function()
{
    return CO_FORSYTHE.getPowerName();
};

