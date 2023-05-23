CO_STURM.superPowerDamage = 8;
CO_STURM.superPowerOffBonus = -20;
CO_STURM.superPowerDefBonus = 30;
CO_STURM.powerDamage = 4;
CO_STURM.powerOffBonus = -20;
CO_STURM.powerDefBonus = 30;
CO_STURM.d2dCoZoneOffBonus = 0;
CO_STURM.d2dCoZoneDefBonus = 0;
CO_STURM.d2dOffBonus = -20;
CO_STURM.d2dDefBonus = 20;

CO_STURM.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_STURM.activateSuperpower = function(co, powerMode, map)
{
    CO_STURM.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_STURM.getSuperPowerDescription = function()
{
    return CO_STURM.getPowerDescription();
};
CO_STURM.getSuperPowerName = function()
{
    return CO_STURM.getPowerName();
};
