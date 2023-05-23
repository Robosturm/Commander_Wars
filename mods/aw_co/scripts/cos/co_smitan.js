CO_SMITAN.superPowerFirerangeModifier = 2;
CO_SMITAN.superPowerOffBonus = 20;
CO_SMITAN.powerOffBonus = 0;
CO_SMITAN.powerBaseOffBonus = 0;
CO_SMITAN.powerDefBonus = 10;
CO_SMITAN.powerFirerangeModifier = 1;
CO_SMITAN.d2dOffBonus = 10;
CO_SMITAN.d2dCoZoneOffBonus = 0;
CO_SMITAN.d2dCoZoneDefBonus = 0;
CO_SMITAN.d2dCoZoneBaseOffBonus = 0;

CO_SMITAN.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_SMITAN.activateSuperpower = function(co, powerMode, map)
{
    CO_SMITAN.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SMITAN.getSuperPowerDescription = function()
{
    return CO_SMITAN.getPowerDescription();
};
CO_SMITAN.getSuperPowerName = function()
{
    return CO_SMITAN.getPowerName();
};
