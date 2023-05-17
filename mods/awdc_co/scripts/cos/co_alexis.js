CO_ALEXIS.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ALEXIS.activateSuperpower = function(co, powerMode, map)
{
    CO_ALEXIS.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_ALEXIS.getSuperPowerDescription = function()
{
    return CO_ALEXIS.getPowerDescription();
};
CO_ALEXIS.getSuperPowerName = function()
{
    return CO_ALEXIS.getPowerName();
};
