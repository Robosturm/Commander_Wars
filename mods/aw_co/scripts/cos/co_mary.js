CO_MARY.superPowerOffBonus = 20;
CO_MARY.superPowerDefenseReduction = 15;
CO_MARY.superPowerCaptureBonus = 15;
CO_MARY.powerOffBonus = 10;
CO_MARY.powerDefBonus = 10;
CO_MARY.d2dCoZoneOffBonus = 0;
CO_MARY.d2dCoZoneDefBonus = 0;
CO_MARY.d2dCaptureBonus = 5;

CO_MARY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_MARY.activateSuperpower = function(co, powerMode, map)
{
    CO_MARY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MARY.getSuperPowerDescription = function()
{
    return CO_MARY.getPowerDescription();
};
CO_MARY.getSuperPowerName = function()
{
    return CO_MARY.getPowerName();
};
