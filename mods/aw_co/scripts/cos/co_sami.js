CO_SAMI.superPowerOffBonus = 80;
CO_SAMI.superpowerMovementPoints = 2;
CO_SAMI.superpowerCaptureMultiplier = 20;
CO_SAMI.powerOffBonus = 20;
CO_SAMI.powerDefBonus = 10;
CO_SAMI.powerBaseOffBonus = 0;
CO_SAMI.powerMovementPoints = 1;
CO_SAMI.powerMovementCostModifier = -998;
CO_SAMI.d2dCoZoneOffBonus = 0;
CO_SAMI.d2dCoZoneBaseOffBonus = 0;
CO_SAMI.d2dCoZoneDefBonus = 0;
CO_SAMI.d2dInfOffBonus = 20;
CO_SAMI.d2dInfDefBonus = 10;
CO_SAMI.d2dDirectOffBonus = -10;
CO_SAMI.d2dCaptureMultiplier = 0.5;
CO_SAMI.d2dTransporterMovementPoints = 1;

CO_SAMI.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_SAMI.activateSuperpower = function(co, powerMode, map)
{
    CO_SAMI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SAMI.getSuperPowerDescription = function()
{
    return CO_SAMI.getPowerDescription();
};
CO_SAMI.getSuperPowerName = function()
{
    return CO_SAMI.getPowerName();
};
