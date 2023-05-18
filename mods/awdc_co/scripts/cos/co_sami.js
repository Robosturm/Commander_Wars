CO_SAMI.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SAMI.activateSuperpower = function(co, powerMode, map)
{
    CO_SAMI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_SAMI.getSuperPowerDescription = function()
{
    return CO_SAMI.getPowerDescription();
};
CO_SAMI.getSuperPowerName = function()
{
    return CO_SAMI.getPowerName();
};
