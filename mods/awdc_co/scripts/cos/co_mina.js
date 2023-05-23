CO_MINA.init = function(co, map)
{
    co.setPowerStars(3);
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
