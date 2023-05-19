CO_GREYFIELD.superPowerMovementBonus = 1;
CO_GREYFIELD.superPowerOffBonus = 60;
CO_GREYFIELD.superPowerDefBonus = 70;

CO_GREYFIELD.powerOffBonus = 20;
CO_GREYFIELD.powerDefBonus = 50;
CO_GREYFIELD.powerBaseOffBonus = 0;
CO_GREYFIELD.powerBaseDefBonus = 10;

CO_GREYFIELD.d2dOffBonus = 0;
CO_GREYFIELD.d2dDefBonus = 20;

CO_GREYFIELD.d2dCoZoneBaseOffBonus = 0;
CO_GREYFIELD.d2dCoZoneBaseDefBonus = 0;
CO_GREYFIELD.d2dCoZoneOffBonus = 0;
CO_GREYFIELD.d2dCoZoneDefBonus = 0;

CO_GREYFIELD.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_GREYFIELD.activateSuperpower = function(co, powerMode, map)
{
    CO_GREYFIELD.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GREYFIELD.getSuperPowerDescription = function()
{
    return CO_GREYFIELD.getPowerDescription();
};
CO_GREYFIELD.getSuperPowerName = function()
{
    return CO_GREYFIELD.getPowerName();
};
