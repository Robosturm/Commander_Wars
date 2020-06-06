CO_IK_486_B7.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_IK_486_B7.activateSuperpower = function(co, powerMode)
{
	CO_IK_486_B7.activatePower(co, powerMode);
};
CO_IK_486_B7.getSuperPowerDescription = function()
{
    return CO_IK_486_B7.getPowerDescription();
};
CO_IK_486_B7.getSuperPowerName = function()
{
    return CO_IK_486_B7.getPowerName();
};