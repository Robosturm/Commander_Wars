CO_ADDER.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ADDER.activateSuperpower = function(co, powerMode)
{
	CO_ADDER.activatePower(co, powerMode);
};
CO_ADDER.getSuperPowerDescription = function()
{
    return CO_ADDER.getPowerDescription();
};
CO_ADDER.getSuperPowerName = function()
{
    return CO_ADDER.getPowerName();
};