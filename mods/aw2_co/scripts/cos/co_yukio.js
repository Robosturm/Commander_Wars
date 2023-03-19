CO_YUKIO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        return 10;
    }
    return 0;
};
CO_YUKIO.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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
                                  defender, defPosX, defPosY, isDefender, action, luckmode, map)
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
                return CO_YUKIO.trueDamageBonus;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (damage >= CO_YUKIO.mintrueDamage)
            {
                return CO_YUKIO.trueDamageBonus;
            }
            break;
        default:
            if (damage >= CO_YUKIO.mintrueDamage)
            {
                return CO_YUKIO.trueDamageBonus;
            }
            break;
        }
    }
    return 0;
};

CO_YUKIO.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                       defender, defPosX, defPosY, isDefender, luckMode, map)
{
    if (co.getIsCO0() === true)
    {
        // reduce counter damage by a flat amount here
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return CO_YUKIO.trueDefenseBonus;
        case GameEnums.PowerMode_Power:
            return CO_YUKIO.trueDefenseBonus;
        default:
            return CO_YUKIO.trueDefenseBonus;
        }
    }
    return 0;
};
