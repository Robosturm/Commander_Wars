CO_EAGLE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_EAGLE.activateSuperpower = function(co, powerMode, map)
{
	CO_EAGLE.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_EAGLE.getSuperPowerDescription = function()
{
    return CO_EAGLE.getPowerDescription();
};
CO_EAGLE.getSuperPowerName = function()
{
    return CO_EAGLE.getPowerName();
};
