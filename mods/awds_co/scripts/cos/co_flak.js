CO_FLAK.getBonusLuck = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 90;
        case GameEnums.PowerMode_Power:
            return 55;
        default:
            return 25;
        }
    }
    return 0;
};

CO_FLAK.getBonusMisfortune = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 40;
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            return 10;
        }
    }
    return 0;
};
