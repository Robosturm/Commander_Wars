CO_HACHI.powerCostReduction = 0.5;
CO_HACHI.powerOffBonus = 0;
CO_HACHI.powerDefBonus = 10;

CO_HACHI.d2dCoZoneOffBonus = 0;
CO_HACHI.d2dCoZoneDefBonus = 0;

CO_HACHI.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_HACHI.activateSuperpower = function(co, powerMode, map)
{
    CO_HACHI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_HACHI.getSuperPowerDescription = function()
{
    return CO_HACHI.getPowerDescription();
};
CO_HACHI.getSuperPowerName = function()
{
    return CO_HACHI.getPowerName();
};
