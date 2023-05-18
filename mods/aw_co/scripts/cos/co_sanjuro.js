CO_SANJURO.superPowerDefBonus = 30;
CO_SANJURO.superPowerOffBonus = 40;
CO_SANJURO.superPowerCostReduction = 0.5;
CO_SANJURO.powerRefund = 0.5;
CO_SANJURO.powerOffBonus = 0;
CO_SANJURO.powerDefBonus = 10;
CO_SANJURO.d2dCoZoneOffBonus = 0;
CO_SANJURO.d2dCoZoneDefBonus = 0;
CO_SANJURO.d2dCostModifier = 0.01;
CO_SANJURO.d2dOffModifier = 1;
CO_SANJURO.d2dExceedBonus = 0.01;
CO_SANJURO.d2dMaxBonus = 20;

CO_SANJURO.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_SANJURO.activateSuperpower = function(co, powerMode, map)
{
    CO_SANJURO.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SANJURO.getSuperPowerDescription = function()
{
    return CO_SANJURO.getPowerDescription();
};
CO_SANJURO.getSuperPowerName = function()
{
    return CO_SANJURO.getPowerName();
};
