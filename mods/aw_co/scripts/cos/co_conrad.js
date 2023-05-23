CO_CONRAD.superPowerBonusLuckDamage = 50;

CO_CONRAD.powerVisionMultiplier = 5;
CO_CONRAD.powerBonusVisionRange = 1;
CO_CONRAD.powerDefBonus = 10;
CO_CONRAD.powerOffBonus = 0;

CO_CONRAD.d2dCoZoneOffBonus = 0;
CO_CONRAD.d2dCoZoneDefBonus = 0;
CO_CONRAD.d2dCoZoneVisionMultiplier = 0;

CO_CONRAD.d2dCounterDamageBonus = -20;
CO_CONRAD.d2dVisionMultiplier = 2;

CO_CONRAD.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_CONRAD.activateSuperpower = function(co, powerMode, map)
{
    CO_CONRAD.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_CONRAD.getSuperPowerDescription = function()
{
    return CO_CONRAD.getPowerDescription();
};
CO_CONRAD.getSuperPowerName = function()
{
    return CO_CONRAD.getPowerName();
};
