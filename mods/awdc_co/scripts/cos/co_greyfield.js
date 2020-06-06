CO_GREYFIELD.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GREYFIELD.activateSuperpower = function(co, powerMode)
{
	CO_GREYFIELD.activatePower(co, powerMode);
};
CO_GREYFIELD.getSuperPowerDescription = function()
{
    return CO_GREYFIELD.getPowerDescription();
};
CO_GREYFIELD.getSuperPowerName = function()
{
    return CO_GREYFIELD.getPowerName();
};