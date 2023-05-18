CO_SANJURO.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SANJURO.activateSuperpower = function(co, powerMode, map)
{
	CO_SANJURO.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SANJURO.getSuperPowerDescription = function()
{
    return CO_SANJURO.getPowerDescription();
};
CO_SANJURO.getSuperPowerName = function()
{
    return CO_SANJURO.getPowerName();
};
