CO_CASSIDY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender)
{
    if (defender !== null)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getHp() > defender.getHp())
            {
                return 80;
            }
            return 30;
        case GameEnums.PowerMode_Power:
            if (attacker.getHp() > defender.getHp())
            {
                return 60;
            }
            return 30;
        default:
            if (attacker.getHp() > defender.getHp())
            {
                return 20;
            }
        }
    }
    return 0;
};
