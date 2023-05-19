CO_GRIMM.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GRIMM.activateSuperpower = function(co, powerMode, map)
{
	CO_GRIMM.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GRIMM.getSuperPowerDescription = function()
{
    return CO_GRIMM.getPowerDescription();
};
CO_GRIMM.getSuperPowerName = function()
{
    return CO_GRIMM.getPowerName();
};
