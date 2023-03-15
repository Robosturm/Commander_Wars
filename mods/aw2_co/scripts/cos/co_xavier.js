CO_XAVIER.superpowerBonus = 60;
CO_XAVIER.firepowerBonus = 0;
CO_XAVIER.luckDamage = 40;

CO_XAVIER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            var count = 0;
            if (CO_XAVIER.isAlliedUnit(attacker, defPosX, defPosY + 1, map))
            {
                count++;
            }
            if (CO_XAVIER.isAlliedUnit(attacker, defPosX, defPosY - 1, map))
            {
                count++;
            }
            if (CO_XAVIER.isAlliedUnit(attacker, defPosX + 1, defPosY, map))
            {
                count++;
            }
            if (CO_XAVIER.isAlliedUnit(attacker, defPosX - 1, defPosY, map))
            {
                count++;
            }
            return CO_XAVIER.superpowerBonus * count;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            break;
        }
    }
    return 0;
};
CO_XAVIER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_XAVIER.getBonusLuck = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return hpRounded / 2;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return CO_XAVIER.luckDamage + hpRounded / 2;
            }
            return CO_XAVIER.luckDamage;
        default:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return hpRounded / 2;
            }
        }
    }
    return 0;
};

CO_XAVIER.getBonusMisfortune = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return -hpRounded;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return -CO_XAVIER.luckDamage + -hpRounded;
            }
            return -CO_XAVIER.luckDamage;
        default:
            if (hpRounded <= CO_XAVIER.minLuckHp)
            {
                return -hpRounded;
            }
            break;
        }
    }
    return 0;
};
