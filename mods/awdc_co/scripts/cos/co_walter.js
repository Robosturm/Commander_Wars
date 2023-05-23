CO_WALTER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_WALTER.activateSuperpower = function(co, powerMode, map)
{
    CO_WALTER.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_WALTER.getSuperPowerDescription = function()
{
    return CO_WALTER.getPowerDescription();
};
CO_WALTER.getSuperPowerName = function()
{
    return CO_WALTER.getPowerName();
};
CO_WALTER.getHpHidden = function(co, unit, posX, posY, map)
{
    return false;
};
