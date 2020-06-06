CO_KANBEI.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_KANBEI.activateSuperpower = function(co, powerMode)
{
	CO_KANBEI.activatePower(co, powerMode);
};
CO_KANBEI.getSuperPowerDescription = function()
{
    return CO_KANBEI.getPowerDescription();
};
CO_KANBEI.getSuperPowerName = function()
{
    return CO_KANBEI.getPowerName();
};