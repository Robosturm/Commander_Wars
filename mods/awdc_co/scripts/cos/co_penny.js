CO_PENNY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_PENNY.activateSuperpower = function(co, powerMode, map)
{
    CO_PENNY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_PENNY.getSuperPowerDescription = function()
{
    return CO_PENNY.getPowerDescription();
};
CO_PENNY.getSuperPowerName = function()
{
    return CO_PENNY.getPowerName();
};
