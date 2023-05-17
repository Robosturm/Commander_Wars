CO_ALEXIS.superPowerOffBonus = 40;
CO_ALEXIS.superPowerOffBaseBonus = 0;
CO_ALEXIS.superPowerHeal = 3;
CO_ALEXIS.superPowerHealRadius = 2;
CO_ALEXIS.superPowerDamage = 3;
CO_ALEXIS.superPowerDamageRadius = 1;
CO_ALEXIS.powerOffBonus = 20;
CO_ALEXIS.powerOffBaseBonus = 0;
CO_ALEXIS.powerDefBonus = 10;
CO_ALEXIS.powerHeal = 3;
CO_ALEXIS.powerRadius = 2;
CO_ALEXIS.coZoneBonus = 0;
CO_ALEXIS.d2dHealBonus = 1;
CO_ALEXIS.d2dHealRadius = 1;
CO_ALEXIS.d2dHealMalus = 1;

CO_ALEXIS.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_ALEXIS.activateSuperpower = function(co, powerMode, map)
{
    CO_ALEXIS.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ALEXIS.getSuperPowerDescription = function()
{
    return CO_ALEXIS.getPowerDescription();
};
CO_ALEXIS.getSuperPowerName = function()
{
    return CO_ALEXIS.getPowerName();
};
