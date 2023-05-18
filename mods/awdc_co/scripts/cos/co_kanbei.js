CO_KANBEI.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_KANBEI.activateSuperpower = function(co, powerMode, map)
{
	CO_KANBEI.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_KANBEI.getSuperPowerDescription = function()
{
    return CO_KANBEI.getPowerDescription();
};
CO_KANBEI.getSuperPowerName = function()
{
    return CO_KANBEI.getPowerName();
};
