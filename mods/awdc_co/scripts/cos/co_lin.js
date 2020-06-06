CO_LIN.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_LIN.activateSuperpower = function(co, powerMode)
{
	CO_LIN.activatePower(co, powerMode);
};
CO_LIN.getSuperPowerDescription = function()
{
    return CO_LIN.getPowerDescription();
};
CO_LIN.getSuperPowerName = function()
{
    return CO_LIN.getPowerName();
};
CO_LIN.getVisionrangeModifier = function(co, unit, posX, posY)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 2;
        default:
            return 0;
    }
};