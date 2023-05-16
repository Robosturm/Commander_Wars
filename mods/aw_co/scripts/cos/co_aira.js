this.superPowerDamage = 3;
this.superPowerMovementCostIncrease = 1;
this.powerFuelIncrease = 5;
this.powerOffBonus = 0;
this.powerDefBonus = 10;
this.coZoneBonus = 0;

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
