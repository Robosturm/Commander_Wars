CO_KANBEI.d2dcoZoneBonus = 0;

CO_KANBEI.d2dBonus = 20;
CO_KANBEI.d2dCostIncrease = 20;

CO_KANBEI.powerOffBonus = 50;
CO_KANBEI.powerDefBonus = 40;

CO_KANBEI.superPowerOffBonus = 50;
CO_KANBEI.superPowerCounterBonus = 100;
CO_KANBEI.superPowerDefBonus = 60;

CO_KANBEI.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_KANBEI.activateSuperpower = function(co, powerMode, map)
{
    CO_KANBEI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_KANBEI.getSuperPowerDescription = function()
{
    return CO_KANBEI.getPowerDescription();
};
CO_KANBEI.getSuperPowerName = function()
{
    return CO_KANBEI.getPowerName();
};

