CO_SASHA.superPowerIncomeBonus = 0.5;
CO_SASHA.powerFundsPerReduction = 5000;
CO_SASHA.d2dPercentIncomeBonus = 0.1;
CO_SASHA.d2dFlatIncomeBonus = 0;
CO_SASHA.powerOffBonus = 0;
CO_SASHA.powerDefBonus = 10;
CO_SASHA.d2dCoZoneOffBonus = 0;
CO_SASHA.d2dCoZoneDefBonus = 0;

CO_SASHA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_SASHA.activateSuperpower = function(co, powerMode, map)
{
    CO_SASHA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SASHA.getSuperPowerDescription = function()
{
    return CO_SASHA.getPowerDescription();
};
CO_SASHA.getSuperPowerName = function()
{
    return CO_SASHA.getPowerName();
};
