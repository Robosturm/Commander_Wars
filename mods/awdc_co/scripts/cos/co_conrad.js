CO_CONRAD.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_CONRAD.activateSuperpower = function(co, powerMode, map)
{
	CO_CONRAD.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_CONRAD.getSuperPowerDescription = function()
{
    return CO_CONRAD.getPowerDescription();
};
CO_CONRAD.getSuperPowerName = function()
{
    return CO_CONRAD.getPowerName();
};
