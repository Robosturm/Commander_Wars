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
            case GameEnums.UnitRank_Lieutenant:
                return 10;
            case GameEnums.UnitRank_General:
                return 20;
            case GameEnums.UnitRank_Veteran:
            case GameEnums.UnitRank_CO0:
            case GameEnums.UnitRank_CO1:
                return 40;
            default:
                return 0;
            }
        case GameEnums.PowerMode_Power:
            switch (attacker.getUnitRank())
            {
            case GameEnums.UnitRank_Lieutenant:
                return 10;
            case GameEnums.UnitRank_General:
                return 20;
            case GameEnums.UnitRank_Veteran:
            case GameEnums.UnitRank_CO0:
            case GameEnums.UnitRank_CO1:
                return 40;
            default:
                return 0;
            }
        default:
            switch (attacker.getUnitRank())
            {
            case GameEnums.UnitRank_Lieutenant:
                return 5;
            case GameEnums.UnitRank_General:
                return 10;
            case GameEnums.UnitRank_Veteran:
            case GameEnums.UnitRank_CO0:
            case GameEnums.UnitRank_CO1:
                return 20;
            default:
                return -5;
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
            case GameEnums.UnitRank_Lieutenant:
                return 10;
            case GameEnums.UnitRank_General:
                return 20;
            case GameEnums.UnitRank_Veteran:
            case GameEnums.UnitRank_CO0:
            case GameEnums.UnitRank_CO1:
                return 40;
            default:
                return 0;
            }
        case GameEnums.PowerMode_Power:
            switch (defender.getUnitRank())
            {
            case GameEnums.UnitRank_Lieutenant:
                return 10;
            case GameEnums.UnitRank_General:
                return 20;
            case GameEnums.UnitRank_Veteran:
            case GameEnums.UnitRank_CO0:
            case GameEnums.UnitRank_CO1:
                return 40;
            default:
                return 0;
            }
        default:
            switch (defender.getUnitRank())
            {
            case GameEnums.UnitRank_Lieutenant:
                return 5;
            case GameEnums.UnitRank_General:
                return 10;
            case GameEnums.UnitRank_Veteran:
            case GameEnums.UnitRank_CO0:
            case GameEnums.UnitRank_CO1:
                return 20;
            default:
                return 0;
            }
        }
    }
    return 0;
};
