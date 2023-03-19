CO_LASH.terrainDefenseModifier = 0;
CO_LASH.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_LASH.activateSuperpower = function(co, powerMode, map)
{
	CO_LASH.activatePower(co, map);
};
CO_LASH.getSuperPowerDescription = function()
{
    return CO_LASH.getPowerDescription();
};
CO_LASH.getSuperPowerName = function()
{
    return CO_LASH.getPowerName();
};
