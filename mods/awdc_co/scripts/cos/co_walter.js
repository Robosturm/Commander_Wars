CO_WALTER.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_WALTER.activateSuperpower = function(co, powerMode)
{
    CO_ADAM.activatePower(co, powerMode);
};
CO_WALTER.getSuperPowerDescription = function()
{
    return CO_ADAM.getPowerDescription();
};
CO_WALTER.getSuperPowerName = function()
{
    return CO_ADAM.getPowerName();
};
