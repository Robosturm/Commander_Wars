CO_ALEXANDER.powerOffBonus = 0;
CO_ALEXANDER.powerDefBonus = 10;
CO_ALEXANDER.powerCaptureBonus = 50;
CO_ALEXANDER.d2dRevoltPoints = 1;
CO_ALEXANDER.d2dCoZoneBonus = 0;

CO_ALEXANDER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_ALEXANDER.activateSuperpower = function(co, powerMode, map)
{
    CO_ALEXANDER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ALEXANDER.getSuperPowerDescription = function()
{
    return CO_ALEXANDER.getPowerDescription();
};
CO_ALEXANDER.getSuperPowerName = function()
{
    return CO_ALEXANDER.getPowerName();
};
