CO_AMY.superPowerDeffensiveBonus = 80;
CO_AMY.powerHoverCraftBoost = 30;
CO_AMY.powerCostReduction = 0.3;
CO_AMY.powerMovementBonus = 2;
CO_AMY.powerOffBonus = 0;
CO_AMY.powerDefBonus = 10;
CO_AMY.d2dCoZoneHoverCraftBoost = 0;
CO_AMY.d2dCoZoneOffBonus = 0;
CO_AMY.d2dCoZoneDefBonus = 0;
CO_AMY.d2dHoverCraftBoost = 20;
CO_AMY.d2dMovementBonus = 1;

CO_AMY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_AMY.activateSuperpower = function(co, powerMode, map)
{
    CO_AMY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_AMY.getSuperPowerDescription = function()
{
    return CO_AMY.getPowerDescription();
};
CO_AMY.getSuperPowerName = function()
{
    return CO_AMY.getPowerName();
};
