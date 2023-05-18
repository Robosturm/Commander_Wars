CO_OLAF.superPowerDamage = 2;
CO_OLAF.superPowerOffBonus = 0;
CO_OLAF.powerOffBonus = 0;
CO_OLAF.powerDefBonus = 10;
CO_OLAF.d2dOffBonus = 0;
CO_OLAF.d2dRainMalus = true;
CO_OLAF.d2dCoZoneOffBonus = 0;
CO_OLAF.d2dCoZoneBaseOffBonus = 0;
CO_OLAF.d2dCoZoneDefBonus = 0;

CO_OLAF.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_OLAF.activateSuperpower = function(co, powerMode, map)
{
    CO_OLAF.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_OLAF.getSuperPowerDescription = function()
{
    return CO_OLAF.getPowerDescription();
};
CO_OLAF.getSuperPowerName = function()
{
    return CO_OLAF.getPowerName();
};
