CO_IK_486_B7.superPowerFirerangeBonus = 3;
CO_IK_486_B7.superPowerOffMalus = -10;
CO_IK_486_B7.superPowerDirectDefBonus = 50;

CO_IK_486_B7.powerOffMalus = -10;
CO_IK_486_B7.powerFirerangeBonus = 2;
CO_IK_486_B7.powerOffBonus = 0;
CO_IK_486_B7.powerDefBonus = 10;
CO_IK_486_B7.powerDirectDefBonus = 30;

CO_IK_486_B7.d2dFirerangeBonus = 1;
CO_IK_486_B7.d2dOffMalus = -10;

CO_IK_486_B7.d2dCoZoneFirerangeBonus = 0;
CO_IK_486_B7.d2dCoZoneOffMalus = 0;
CO_IK_486_B7.d2dCoZoneOffBonus = 0;
CO_IK_486_B7.d2dCoZoneDefBonus = 0;

CO_IK_486_B7.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_IK_486_B7.activateSuperpower = function(co, powerMode, map)
{
    CO_IK_486_B7.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_IK_486_B7.getSuperPowerDescription = function()
{
    return CO_IK_486_B7.getPowerDescription();
};
CO_IK_486_B7.getSuperPowerName = function()
{
    return CO_IK_486_B7.getPowerName();
};
