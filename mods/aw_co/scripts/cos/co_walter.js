CO_WALTER.superPowerDamage = 1;
CO_WALTER.powerOffBonus = 0;
CO_WALTER.powerDefBonus = 10;
CO_WALTER.d2dCoZoneOffBonus = 0;
CO_WALTER.d2dCoZoneDefBonus = 0;

CO_WALTER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_WALTER.activateSuperpower = function(co, powerMode, map)
{
    CO_WALTER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_WALTER.getSuperPowerDescription = function()
{
    return CO_WALTER.getPowerDescription();
};
CO_WALTER.getSuperPowerName = function()
{
    return CO_WALTER.getPowerName();
};
