CO_FLAK.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_FLAK.activateSuperpower = function(co, powerMode, map)
{
	CO_FLAK.activatePower(co, powerMode);
};
CO_FLAK.getSuperPowerDescription = function()
{
    return CO_FLAK.getPowerDescription();
};
CO_FLAK.getSuperPowerName = function()
{
    return CO_FLAK.getPowerName();
};
CO_FLAK.getBonusLuck = function(co, unit, posX, posY, map)
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
                return 25;
            }
            break;
    }
    return 10;
};

CO_FLAK.getBonusMisfortune = function(co, unit, posX, posY, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            if (co.inCORange(Qt.point(posX, posY), unit))
            {
                return 10;
            }
            break;
    }
    return 5;
};
