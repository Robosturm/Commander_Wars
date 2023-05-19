CO_GRIT.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GRIT.activateSuperpower = function(co, powerMode, map)
{
	CO_GRIT.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GRIT.getSuperPowerDescription = function()
{
    return CO_GRIT.getPowerDescription();
};
CO_GRIT.getSuperPowerName = function()
{
    return CO_GRIT.getPowerName();
};
