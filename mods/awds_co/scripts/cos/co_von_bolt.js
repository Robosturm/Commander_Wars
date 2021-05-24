CO_VON_BOLT.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 20;
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            return 10;
        }
    }
    return 0;
};
CO_VON_BOLT.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 20;
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            return 10;
        }
    }
    return 0;
};
