CO_MEIYO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            switch (attacker.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        case GameEnums.PowerMode_Power:
            switch (attacker.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        default:
            switch (attacker.getUnitRank())
            {
            case 0:
                return 5;
            case 1:
                return 15;
            case 2:
                return 20;
            default:
                return 30;
            }
        }
    }
    return 0;
};

CO_MEIYO.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            switch (defender.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        case GameEnums.PowerMode_Power:
            switch (defender.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        default:
            switch (defender.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 15;
            case 2:
                return 20;
            default:
                return 30;
            }
        }
    }
    return 0;
};
