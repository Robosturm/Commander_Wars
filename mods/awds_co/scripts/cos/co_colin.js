CO_COLIN.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            var bonus = attacker.getOwner().getFonds() / 1000 * 3.33 - 15;
            return bonus;
        case GameEnums.PowerMode_Power:
            break;
        default:
            return -10;
        }
    }
    return 0;
};
