CO_ZANDRA.superPowerSandstormDamage = 2;
CO_ZANDRA.powerSandstormBonus = 30;
CO_ZANDRA.powerSandstormRangeBonus = 1;
CO_ZANDRA.powerOffBonus = 10;
CO_ZANDRA.powerDefBonus = 10;
CO_ZANDRA.d2dSandStormBonus = 0;
CO_ZANDRA.d2dCoZoneSandStormBonus = 30;
CO_ZANDRA.d2dCoZoneOffBonus = 0;
CO_ZANDRA.d2dCoZoneDefBonus = 0;

CO_ZANDRA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_ZANDRA.activateSuperpower = function(co, powerMode, map)
{
    CO_ZANDRA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ZANDRA.getSuperPowerDescription = function()
{
    return CO_ZANDRA.getPowerDescription();
};
CO_ZANDRA.getSuperPowerName = function()
{
    return CO_ZANDRA.getPowerName();
};
