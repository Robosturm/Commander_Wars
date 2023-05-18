CO_MINA.superPowerDamage = 5;
CO_MINA.powerOffBonus = 0;
CO_MINA.powerDefBonus = 10;
CO_MINA.powerDamageReduction = 50;

CO_MINA.d2dCoZoneOffBonus = 0;
CO_MINA.d2dCoZoneDefBonus = 0;

CO_MINA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_MINA.activateSuperpower = function(co, powerMode, map)
{
    CO_MINA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_MINA.getSuperPowerDescription = function()
{
    return CO_MINA.getPowerDescription();
};
CO_MINA.getSuperPowerName = function()
{
    return CO_MINA.getPowerName();
};
