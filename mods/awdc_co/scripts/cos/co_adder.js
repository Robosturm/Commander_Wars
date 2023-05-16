CO_ADDER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ADDER.activateSuperpower = function(co, powerMode, map)
{
	CO_ADDER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ADDER.getSuperPowerDescription = function()
{
    return CO_ADDER.getPowerDescription();
};
CO_ADDER.getSuperPowerName = function()
{
    return CO_ADDER.getPowerName();
};
