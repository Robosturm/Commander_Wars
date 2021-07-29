CO_OZZY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (isDefender)
            {
                return 90;
            }
            return 0;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            break;
        }
    }
    return 0;
};
CO_OZZY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isAttacker, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 200;
        case GameEnums.PowerMode_Power:
            if (isAttacker)
            {
                return 60;
            }
            else
            {
                return 100;
            }
        default:
            return 15;
        }
    }
    return 0;
};
