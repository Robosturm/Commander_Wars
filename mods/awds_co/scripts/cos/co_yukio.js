CO_YUKIO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 10;
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            return 10;
        }
    }
    return 0;
};
CO_YUKIO.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_YUKIO.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        // reduce counter damage by a flat amount here
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (damage >= CO_YUKIO.mintrueDamage)
            {
                return 15
            }
            break;
        case GameEnums.PowerMode_Power:
            if (damage >= CO_YUKIO.mintrueDamage)
            {
                return 15
            }
            break;
        default:
            if (damage >= CO_YUKIO.mintrueDamage)
            {
                return 15
            }
            break;
        }
    }
    return 0;
};

CO_YUKIO.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                       defender, defPosX, defPosY, isDefender, luckMode)
{
    if (co.getIsCO0() === true)
    {
        // reduce counter damage by a flat amount here
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 15;
        case GameEnums.PowerMode_Power:
            return 15;
        default:
            return 10;
        }
    }
    return 0;
};
