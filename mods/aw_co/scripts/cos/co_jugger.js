CO_JUGGER.superPowerBonusLuck = 95;
CO_JUGGER.superPowerBonusMissfortune = 45;

CO_JUGGER.powerBonusLuck = 55;
CO_JUGGER.powerBonusMissfortune = 25;

CO_JUGGER.powerOffBonus = 0;
CO_JUGGER.powerDefBonus = 10;

CO_JUGGER.d2dCoZoneBonusLuck = 0;
CO_JUGGER.d2dCoZoneBonusMissfortune = 0;
CO_JUGGER.d2dCoZoneOffBonus = 0;
CO_JUGGER.d2dCoZoneDefBonus = 0;

CO_JUGGER.d2dBonusLuck = 30;
CO_JUGGER.d2dBonusMissfortune = 15;

CO_JUGGER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_JUGGER.activateSuperpower = function(co, powerMode, map)
{
    CO_JUGGER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JUGGER.getSuperPowerDescription = function()
{
    return CO_JUGGER.getPowerDescription();
};
CO_JUGGER.getSuperPowerName = function()
{
    return CO_JUGGER.getPowerName();
};
