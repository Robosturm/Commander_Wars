CO_JUGGER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JUGGER.activateSuperpower = function(co, powerMode, map)
{
	CO_JUGGER.activatePower(co, powerMode);
};
CO_JUGGER.getSuperPowerDescription = function()
{
    return CO_JUGGER.getPowerDescription();
};
CO_JUGGER.getSuperPowerName = function()
{
    return CO_JUGGER.getPowerName();
};
CO_JUGGER.getBonusLuck = function(co, unit, posX, posY, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 55;
        default:
            if (co.inCORange(Qt.point(posX, posY), unit))
            {
                return 30;
            }
            break;
    }
    return 14;
};

CO_JUGGER.getBonusMisfortune = function(co, unit, posX, posY, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 25;
        default:
            if (co.inCORange(Qt.point(posX, posY), unit))
            {
                return 15;
            }
            break;
    }
    return 7;
};
