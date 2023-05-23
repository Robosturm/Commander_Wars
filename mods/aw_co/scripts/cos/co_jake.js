CO_JAKE.superPowerMovementBonus = 2;
CO_JAKE.superPowerPlainsBonus = 50;

CO_JAKE.powerFirerangeBonus = 1;
CO_JAKE.powerPlainsBonus = 30;
CO_JAKE.powerDefBonus = 10;
CO_JAKE.powerOffBonus = 0;

CO_JAKE.d2dPlainsBonus = 10;

CO_JAKE.d2dCoZoneOffBonus = 0;
CO_JAKE.d2dCoZoneDefBonus = 0;
CO_JAKE.d2dCoZonePlainsBonus = 0;

CO_JAKE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_JAKE.activateSuperpower = function(co, powerMode, map)
{
    CO_JAKE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JAKE.getSuperPowerDescription = function()
{
    return CO_JAKE.getPowerDescription();
};
CO_JAKE.getSuperPowerName = function()
{
    return CO_JAKE.getPowerName();
};
