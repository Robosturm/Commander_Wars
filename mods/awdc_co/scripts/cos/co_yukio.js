CO_YUKIO.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_YUKIO.activateSuperpower = function(co, powerMode, map)
{
	CO_YUKIO.activatePower(co, map);
};
CO_YUKIO.getSuperPowerDescription = function()
{
    return CO_YUKIO.getPowerDescription();
};
CO_YUKIO.getSuperPowerName = function()
{
    return CO_YUKIO.getPowerName();
};
