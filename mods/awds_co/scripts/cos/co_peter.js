CO_PETER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (defender === null)
        {
            return 0;
        }
        var bonus = 0;
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (defender.getBaseMaxRange() === 1)
            {
                bonus += 40;
            }
            bonus += 10;
            break;
        case GameEnums.PowerMode_Power:
            if (defender.getBaseMaxRange() === 1)
            {
                bonus += 60;
            }
            bonus += 10;
            break;
        default:
            bonus = 20;
            break;
        }
        bonus -= defender.getTerrainDefense() * 5;
        return bonus;
    }
    return 0;
};
CO_PETER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
    }
    return 0;
};
