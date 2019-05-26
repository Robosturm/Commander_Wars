
CO_FORSYTHE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 30;
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            return 5;
        }
    }
    return 0;
};

CO_FORSYTHE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                   defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 30;
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            break;
        }
    }
    return 0;
};
