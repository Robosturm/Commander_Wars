CO_COLIN.superPowerOffBonus = 3.33;

CO_COLIN.powerFundsMultiplier = 0.5;
CO_COLIN.powerOffBonus = -10;
CO_COLIN.powerDefBonus = 10;

CO_COLIN.d2dCostModifier = -20;
CO_COLIN.d2dOffBonus = -10;

CO_COLIN.d2dCoZoneOffBonus = 0;
CO_COLIN.d2dCoZoneDefBonus = 0;

CO_COLIN.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_COLIN.activateSuperpower = function(co, powerMode, map)
{
    CO_COLIN.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_COLIN.getSuperPowerDescription = function()
{
    return CO_COLIN.getPowerDescription();
};
CO_COLIN.getSuperPowerName = function()
{
    return CO_COLIN.getPowerName();
};
