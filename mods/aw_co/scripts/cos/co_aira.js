CO_AIRA.superPowerDamage = 3;
CO_AIRA.superPowerMovementCostIncrease = 1;
CO_AIRA.powerFuelIncrease = 5;
CO_AIRA.powerOffBonus = 0;
CO_AIRA.powerDefBonus = 10;
CO_AIRA.coZoneBonus = 0;

CO_AIRA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_AIRA.activateSuperpower = function(co, powerMode, map)
{
    CO_AIRA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_AIRA.getSuperPowerDescription = function()
{
    return CO_AIRA.getPowerDescription();
};
CO_AIRA.getSuperPowerName = function()
{
    return CO_AIRA.getPowerName();
};
