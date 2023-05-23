CO_FLAK.superPowerBonusLuck = 80;
CO_FLAK.superPowerBonusMissfortune = 30;

CO_FLAK.powerBonusLuck = 40;
CO_FLAK.powerBonusMissfortune = 20;

CO_FLAK.powerOffBonus = 0;
CO_FLAK.powerDefBonus = 10;

CO_FLAK.d2dCoZoneBonusLuck = 0;
CO_FLAK.d2dCoZoneBonusMissfortune = 0;
CO_FLAK.d2dCoZoneOffBonus = 0;
CO_FLAK.d2dCoZoneDefBonus = 0;

CO_FLAK.d2dBonusLuck = 15;
CO_FLAK.d2dBonusMissfortune = 10;

CO_FLAK.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_FLAK.activateSuperpower = function(co, powerMode, map)
{
    CO_FLAK.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_FLAK.getSuperPowerDescription = function()
{
    return CO_FLAK.getPowerDescription();
};
CO_FLAK.getSuperPowerName = function()
{
    return CO_FLAK.getPowerName();
};
