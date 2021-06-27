CO_BEAST.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_BEAST.activateSuperpower = function(co, powerMode)
{
    CO_BEAST.activatePower(co, powerMode);
};
CO_BEAST.getSuperPowerDescription = function()
{
    return CO_BEAST.getPowerDescription();
};
CO_BEAST.getSuperPowerName = function()
{
    return CO_BEAST.getPowerName();
};
CO_BEAST.getMovementpointModifier = function(co, unit, posX, posY)
{
    return 0;
};

