CO_BEAST.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_BEAST.activateSuperpower = function(co, powerMode, map)
{
    CO_BEAST.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_BEAST.getSuperPowerDescription = function()
{
    return CO_BEAST.getPowerDescription();
};
CO_BEAST.getSuperPowerName = function()
{
    return CO_BEAST.getPowerName();
};
CO_BEAST.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    return 0;
};

