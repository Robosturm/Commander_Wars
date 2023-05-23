CO_VON_BOLT.superPowerDamage = 3;
CO_VON_BOLT.superPowerRange = 3;
CO_VON_BOLT.powerDamage = 1;
CO_VON_BOLT.powerRange = 2;
CO_VON_BOLT.powerOffBonus = 10;
CO_VON_BOLT.powerDefBonus = 20;
CO_VON_BOLT.d2dCoZoneOffBonus = 0;
CO_VON_BOLT.d2dCoZoneDefBonus = 0;
CO_VON_BOLT.d2dOffBonus = 10;
CO_VON_BOLT.d2dDefBonus = 10;

CO_VON_BOLT.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_VON_BOLT.activateSuperpower = function(co, powerMode, map)
{
    CO_VON_BOLT.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_VON_BOLT.getSuperPowerDescription = function()
{
    return CO_VON_BOLT.getPowerDescription();
};
CO_VON_BOLT.getSuperPowerName = function()
{
    return CO_VON_BOLT.getPowerName();
};
