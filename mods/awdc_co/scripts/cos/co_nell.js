CO_NELL.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_NELL.activateSuperpower = function(co, powerMode, map)
{
	CO_NELL.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_NELL.getSuperPowerDescription = function()
{
    return CO_NELL.getPowerDescription();
};
CO_NELL.getSuperPowerName = function()
{
    return CO_NELL.getPowerName();
};
