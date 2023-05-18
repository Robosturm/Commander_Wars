CO_TASHA.powerMovementBonus = 2;

CO_TASHA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_TASHA.activateSuperpower = function(co, powerMode, map)
{
    CO_TASHA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_TASHA.getSuperPowerDescription = function()
{
    return CO_TASHA.getPowerDescription();
};
CO_TASHA.getSuperPowerName = function()
{
    return CO_TASHA.getPowerName();
};
