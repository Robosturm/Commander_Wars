CO_VON_BOLT.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_VON_BOLT.activateSuperpower = function(co, powerMode, map)
{
	CO_VON_BOLT.activatePower(co, powerMode, map);
};
CO_VON_BOLT.getSuperPowerDescription = function()
{
    return CO_VON_BOLT.getPowerDescription();
};
CO_VON_BOLT.getSuperPowerName = function()
{
    return CO_VON_BOLT.getPowerName();
};
