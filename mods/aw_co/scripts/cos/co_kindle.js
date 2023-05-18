CO_KINDLE.superPowerOffBonus = 120;
CO_KINDLE.superPowerOffMultiplier = 3;

CO_KINDLE.powerDamage = 3;
CO_KINDLE.powerOffBonus = 80;
CO_KINDLE.powerBaseOffBonus = 10;
CO_KINDLE.powerDefBonus = 10;

CO_KINDLE.d2dOffBonus = 40;

CO_KINDLE.d2dCoZoneOffBonus = 0;
CO_KINDLE.d2dCoZoneBaseOffBonus = 0;
CO_KINDLE.d2dCoZoneDefBonus = 0;

CO_KINDLE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_KINDLE.activateSuperpower = function(co, powerMode, map)
{
    CO_KINDLE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_KINDLE.getSuperPowerDescription = function()
{
    return CO_KINDLE.getPowerDescription();
};
CO_KINDLE.getSuperPowerName = function()
{
    return CO_KINDLE.getPowerName();
};
