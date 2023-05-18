CO_VARLOT.superPowerHeal = 4;
CO_VARLOT.superPowerOverhealOffBonus = 10;
CO_VARLOT.powerCaptureBonus = 5;
CO_VARLOT.powerOffBonus = 0;
CO_VARLOT.powerDefBonus = 10;
CO_VARLOT.d2dOffBonus = 0;
CO_VARLOT.d2dDefBonus = 0;
CO_VARLOT.d2dCoZoneOffBonus = 0;
CO_VARLOT.d2dCoZoneDefBonus = 0;

CO_VARLOT.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_VARLOT.activateSuperpower = function(co, powerMode, map)
{
    CO_VARLOT.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_VARLOT.getSuperPowerDescription = function()
{
    return CO_VARLOT.getPowerDescription();
};
CO_VARLOT.getSuperPowerName = function()
{
    return CO_VARLOT.getPowerName();
};
