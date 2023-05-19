CO_ISABELLAthis.superPowerOffBonus = 30;
CO_ISABELLA.superPowerDefBonus = 30;
CO_ISABELLA.superPowerFirerangeBonus = 2;
CO_ISABELLA.superPowerMovementBonus = 2;

CO_ISABELLA.powerOffBonus = 20;
CO_ISABELLA.powerDefBonus = 20;
CO_ISABELLA.powerFirerangeBonus = 1;
CO_ISABELLA.powerMovementBonus = 1;

CO_ISABELLA.d2dOffBonus = 10;
CO_ISABELLA.d2dDefBonus = 10;

CO_ISABELLA.d2dCoZoneOffBonus = 0;
CO_ISABELLA.d2dCoZoneDefBonus = 0;

CO_ISABELLA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_ISABELLA.activateSuperpower = function(co, powerMode, map)
{
    CO_ISABELLA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ISABELLA.getSuperPowerDescription = function()
{
    return CO_ISABELLA.getPowerDescription();
};
CO_ISABELLA.getSuperPowerName = function()
{
    return CO_ISABELLA.getPowerName();
};
