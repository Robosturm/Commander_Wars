CO_NANA.superPowerExplosionRange = 2;
CO_NANA.superPowerExplosionDamage = 2;
CO_NANA.powerOffBonus = 0;
CO_NANA.powerDefBonus = 10;
CO_NANA.powerExplosionDamage = 2;

CO_NANA.d2dCoZoneOffBonus = 0;
CO_NANA.d2dCoZoneDefBonus = 0;
CO_NANA.d2dCoZoneExplosionDamage = 0;

CO_NANA.d2dExplosionRange = 1;
CO_NANA.d2dExplosionDamage = 1;

CO_NANA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_NANA.activateSuperpower = function(co, powerMode, map)
{
    CO_NANA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_NANA.getSuperPowerDescription = function()
{
    return CO_NANA.getPowerDescription();
};
CO_NANA.getSuperPowerName = function()
{
    return CO_NANA.getPowerName();
};
