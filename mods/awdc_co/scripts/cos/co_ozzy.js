CO_OZZY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_OZZY.activateSuperpower = function(co, powerMode, map)
{
	CO_OZZY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_OZZY.getSuperPowerDescription = function()
{
    return CO_OZZY.getPowerDescription();
};
CO_OZZY.getSuperPowerName = function()
{
    return CO_OZZY.getPowerName();
};
