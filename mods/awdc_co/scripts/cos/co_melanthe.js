CO_MELANTHE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MELANTHE.activateSuperpower = function(co, powerMode, map)
{
	CO_MELANTHE.activatePower(co, powerMode, map);
};
CO_MELANTHE.getSuperPowerDescription = function()
{
    return CO_MELANTHE.getPowerDescription();
};
CO_MELANTHE.getSuperPowerName = function()
{
    return CO_MELANTHE.getPowerName();
};
