CO_LIN.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_LIN.activateSuperpower = function(co, powerMode, map)
{
	CO_LIN.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_LIN.getSuperPowerDescription = function()
{
    return CO_LIN.getPowerDescription();
};
CO_LIN.getSuperPowerName = function()
{
    return CO_LIN.getPowerName();
};
CO_LIN.getVisionrangeModifier = function(co, unit, posX, posY, map)
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
