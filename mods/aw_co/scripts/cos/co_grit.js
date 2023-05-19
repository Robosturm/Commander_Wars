CO_GRIT.superPowerFirerangeBonus = 3;
CO_GRIT.superPowerOffBonus = 50;
CO_GRIT.superPowerDirectMalus = -20;
CO_GRIT.superPowerInfMalus = -20;

CO_GRIT.powerFirerangeBonus = 2;
CO_GRIT.powerOffBonus = 50;
CO_GRIT.powerDirectMalus = -20;
CO_GRIT.powerInfMalus = -20;
CO_GRIT.powerDefBonus = 10;
CO_GRIT.powerOffBonus = 0;

CO_GRIT.d2dFirerangeBonus = 1;
CO_GRIT.d2dOffBonus = 20;
CO_GRIT.d2dDirectMalus = -20;
CO_GRIT.d2dInfMalus = -20;

CO_GRIT.d2dCoZoneOffBonus = 0;
CO_GRIT.d2dCoZoneDirectMalus = 0;
CO_GRIT.d2dCoZoneInfMalus = 0;
CO_GRIT.d2dCoZoneOffBonus = 0;
CO_GRIT.d2dCoZoneDefBonus = 0;

CO_GRIT.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_GRIT.activateSuperpower = function(co, powerMode, map)
{
    CO_GRIT.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GRIT.getSuperPowerDescription = function()
{
    return CO_GRIT.getPowerDescription();
};
CO_GRIT.getSuperPowerName = function()
{
    return CO_GRIT.getPowerName();
};
