CO_PETER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_PETER.activateSuperpower = function(co, powerMode, map)
{
    CO_PETER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_PETER.getSuperPowerDescription = function()
{
    return CO_PETER.getPowerDescription();
};
CO_PETER.getSuperPowerName = function()
{
    return CO_PETER.getPowerName();
};
