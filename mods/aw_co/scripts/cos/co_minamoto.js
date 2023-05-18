CO_MINAMOTO.superPowerOffBonus = 40;
CO_MINAMOTO.superPowerMovementPoints = 2;
CO_MINAMOTO.superPowerBlowHp = 3.5;
CO_MINAMOTO.superPowerBlowRange = 4;

CO_MINAMOTO.powerBlowHp = 4.5;
CO_MINAMOTO.powerBlowRange = 2;
CO_MINAMOTO.powerOffBonus = 40;
CO_MINAMOTO.powerBaseOffBonus = 0;
CO_MINAMOTO.powerDefBonus = 10;

CO_MINAMOTO.d2dOffBonus = 20;

CO_MINAMOTO.d2dCoZoneOffBonus = 0;
CO_MINAMOTO.d2dCoZoneBaseOffBonus = 0;
CO_MINAMOTO.d2dCoZoneDefBonus = 0;

CO_MINAMOTO.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_MINAMOTO.activateSuperpower = function(co, powerMode, map)
{
    CO_MINAMOTO.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MINAMOTO.getSuperPowerDescription = function()
{
    return CO_MINAMOTO.getPowerDescription();
};
CO_MINAMOTO.getSuperPowerName = function()
{
    return CO_MINAMOTO.getPowerName();
};
