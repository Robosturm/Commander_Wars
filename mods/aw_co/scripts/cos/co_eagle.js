CO_EAGLE.superPowerAirOffBonus = 30;
CO_EAGLE.superPowerNavalOffBonus = -30;
CO_EAGLE.superPowerOtherOffBonus = 10;
CO_EAGLE.superPowerAirDefBonus = 10;
CO_EAGLE.superPowerDefBonus = 10;

CO_EAGLE.powerAirOffBonus = -5;
CO_EAGLE.powerNavalOffBonus = -40;
CO_EAGLE.powerInfOffBonus = 0;
CO_EAGLE.powerOtherOffBonus = -20;
CO_EAGLE.powerDefBonus = -30;
CO_EAGLE.powerAirDefBonus = -30;
CO_EAGLE.powerRefresh = true;

CO_EAGLE.d2dCoZoneAirDefBonus = 0;
CO_EAGLE.d2dCoZoneDefBonus = 0;
CO_EAGLE.d2dCoZoneAirOffBonus = 0;
CO_EAGLE.d2dCoZoneNavalOffBonus = 0;
CO_EAGLE.d2dCoZoneOtherOffBonus = 0;

CO_EAGLE.d2dFuelModifier = -2;
CO_EAGLE.d2dAirDefBonus = 10;
CO_EAGLE.d2dAirOffBonus = 15;
CO_EAGLE.d2dNavalOffBonus = -20;
CO_EAGLE.d2dOtherOffBonus = 0;

CO_EAGLE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_EAGLE.activateSuperpower = function(co, powerMode, map)
{
    CO_EAGLE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_EAGLE.getSuperPowerDescription = function()
{
    return CO_EAGLE.getPowerDescription();
};
CO_EAGLE.getSuperPowerName = function()
{
    return CO_EAGLE.getPowerName();
};
