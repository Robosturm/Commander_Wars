CO_PENNY.superPowerOffBonus = 20;
CO_PENNY.superPowerFirerangeModifier = 2;
CO_PENNY.superPowerMovementPointBonus = 2;
CO_PENNY.superPowerRainVisionBonus = 3;
CO_PENNY.superPowerMistVisionBonus = 5;
CO_PENNY.powerOffBonus = 0;
CO_PENNY.powerDefBonus = 10;
CO_PENNY.d2dCoZoneOffBonus = 0;
CO_PENNY.d2dCoZoneDefBonus = 0;

CO_PENNY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_PENNY.activateSuperpower = function(co, powerMode, map)
{
    CO_PENNY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_PENNY.getSuperPowerDescription = function()
{
    return CO_PENNY.getPowerDescription();
};
CO_PENNY.getSuperPowerName = function()
{
    return CO_PENNY.getPowerName();
};
