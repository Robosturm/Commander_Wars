CO_MINA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_MINA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_MINA.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                      defender, defPosX, defPosY, isDefender, luckMode)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            return 0;
        case GameEnums.PowerMode_Power:
            if (luckMode === GameEnums.LuckDamageMode_On)
            {
                return globals.randInt(0, 50);
            }
            else if (luckMode === GameEnums.LuckDamageMode_Average)
            {
                return 25;
            }
            return 0;
        default:
            break;
        }
    }
    return 0;
};
