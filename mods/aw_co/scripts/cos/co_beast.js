CO_BEAST.superPowerHeal = 3;
CO_BEAST.superPowerMovementBonus = 2;

CO_BEAST.powerHeal = 2;
CO_BEAST.powerOffBonus = 60;
CO_BEAST.powerDefBonus = 10;

CO_BEAST.d2dCoZoneBaseOffBonus = 0;
CO_BEAST.d2dCoZoneOffBonus = 0;
CO_BEAST.d2dCoZoneDefBonus = 0;

CO_BEAST.d2dOffBonus = 30;
CO_BEAST.d2dSelfDamage = 1;

CO_BEAST.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_BEAST.activateSuperpower = function(co, powerMode, map)
{
    CO_BEAST.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_BEAST.getSuperPowerDescription = function()
{
    return CO_BEAST.getPowerDescription();
};
CO_BEAST.getSuperPowerName = function()
{
    return CO_BEAST.getPowerName();
};
