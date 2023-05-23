CO_ADDER.d2dPowerChargeBonus = 0;
CO_ADDER.d2dCoZoneBonus = 0;
CO_ADDER.powerOffBonus = 0;
CO_ADDER.powerDefBonus = 10;

CO_ADDER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_ADDER.activateSuperpower = function(co, powerMode, map)
{
    CO_ADDER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ADDER.getSuperPowerDescription = function()
{
    return CO_ADAM.getPowerDescription();
};
CO_ADDER.getSuperPowerName = function()
{
    return CO_ADDER.getPowerName();
};
