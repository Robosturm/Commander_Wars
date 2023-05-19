CO_GRIMM.superPowerOffBonus = 80;

CO_GRIMM.powerOffBonus = 50;
CO_GRIMM.powerDefBonus = -10;

CO_GRIMM.d2dCoZoneOffBonus = 0;
CO_GRIMM.d2dCoZoneDefBonus = 0;

CO_GRIMM.d2dOffBonus = 30;
CO_GRIMM.d2dDefBonus = -20;

CO_GRIMM.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_GRIMM.activateSuperpower = function(co, powerMode, map)
{
    CO_GRIMM.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GRIMM.getSuperPowerDescription = function()
{
    return CO_GRIMM.getPowerDescription();
};
CO_GRIMM.getSuperPowerName = function()
{
    return CO_GRIMM.getPowerName();
};
