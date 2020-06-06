CO_ISABELLA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ISABELLA.activateSuperpower = function(co, powerMode)
{
	CO_ISABELLA.activatePower(co, powerMode);
};
CO_ISABELLA.getFirerangeModifier = function(co, unit, posX, posY)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1)
            {
                return 2;
            }
            return 0;
        default:
            return 0;
    }
};
CO_ISABELLA.getMovementpointModifier = function(co, unit, posX, posY)
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