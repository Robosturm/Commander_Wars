CO_ADAM.superPowerOffBaseBonus = 10;
CO_ADAM.superPowerOffDestroyedBonus = 15;
CO_ADAM.superPowerOffMalus = 45;
CO_ADAM.powerBaseBonus = 20;
CO_ADAM.powerDestroyedBonus = 10;
CO_ADAM.powerMovementBonus = 1;
CO_ADAM.powerDefBonus = 10;
CO_ADAM.d2dCoZoneDefBonus = 0;
CO_ADAM.d2dCoZoneOffBonus = 0;
CO_ADAM.d2dPowerMalus = 2;
CO_ADAM.d2dDefDestroyedBonus = 15;

CO_ADAM.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_ADAM.activateSuperpower = function(co, powerMode, map)
{
    CO_ADAM.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ADAM.getSuperPowerDescription = function()
{
    return CO_ADAM.getPowerDescription();
};
CO_ADAM.getSuperPowerName = function()
{
    return CO_ADAM.getPowerName();
};
