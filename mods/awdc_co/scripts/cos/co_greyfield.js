CO_GREYFIELD.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GREYFIELD.activateSuperpower = function(co, powerMode, map)
{
	CO_GREYFIELD.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_GREYFIELD.getSuperPowerDescription = function()
{
    return CO_GREYFIELD.getPowerDescription();
};
CO_GREYFIELD.getSuperPowerName = function()
{
    return CO_GREYFIELD.getPowerName();
};
