CO_OZZY.superPowerOffCounterBonus = 90;
CO_OZZY.superPowerDefBonus = 200;
CO_OZZY.powerOffBonus = 0;
CO_OZZY.powerAtkDefBonus = 60;
CO_OZZY.powerDefBonus = 100;
CO_OZZY.d2dDefBonus = 15;
CO_OZZY.d2dCoZoneOffBonus = 0;
CO_OZZY.d2dCoZoneDefBonus = 0;

CO_OZZY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_OZZY.activateSuperpower = function(co, powerMode, map)
{
    CO_OZZY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_OZZY.getSuperPowerDescription = function()
{
    return CO_OZZY.getPowerDescription();
};
CO_OZZY.getSuperPowerName = function()
{
    return CO_OZZY.getPowerName();
};
