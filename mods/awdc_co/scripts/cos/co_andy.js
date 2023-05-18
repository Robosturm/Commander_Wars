CO_ANDY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ANDY.activateSuperpower = function(co, powerMode, map)
{
    CO_ANDY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ANDY.getSuperPowerDescription = function()
{
    return CO_ANDY.getPowerDescription();
};
CO_ANDY.getSuperPowerName = function()
{
    return CO_ANDY.getPowerName();
};
