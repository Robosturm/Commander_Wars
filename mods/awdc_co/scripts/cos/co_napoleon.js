CO_NAPOLEON.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_NAPOLEON.activateSuperpower = function(co, powerMode, map)
{
    CO_NAPOLEON.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_NAPOLEON.getSuperPowerDescription = function()
{
    return CO_NAPOLEON.getPowerDescription();
};
CO_NAPOLEON.getSuperPowerName = function()
{
    return CO_NAPOLEON.getPowerName();
};
