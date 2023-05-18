CO_BRENNER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_BRENNER.activateSuperpower = function(co, powerMode, map)
{
	CO_BRENNER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_BRENNER.getSuperPowerDescription = function()
{
    return CO_BRENNER.getPowerDescription();
};
CO_BRENNER.getSuperPowerName = function()
{
    return CO_BRENNER.getPowerName();
};
