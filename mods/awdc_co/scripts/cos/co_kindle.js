CO_KINDLE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_KINDLE.activateSuperpower = function(co, powerMode, map)
{
	CO_KINDLE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_KINDLE.getSuperPowerDescription = function()
{
    return CO_KINDLE.getPowerDescription();
};
CO_KINDLE.getSuperPowerName = function()
{
    return CO_KINDLE.getPowerName();
};
