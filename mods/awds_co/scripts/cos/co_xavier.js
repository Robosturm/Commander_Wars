CO_XAVIER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            var count = 0;
            if (CO_XAVIER.isAlliedUnit(attacker, defPosX, defPosY + 1))
            {
                count++;
            }
            if (CO_XAVIER.isAlliedUnit(attacker, defPosX, defPosY - 1))
            {
                count++;
            }
            if (CO_XAVIER.isAlliedUnit(attacker, defPosX + 1, defPosY))
            {
                count++;
            }
            if (CO_XAVIER.isAlliedUnit(attacker, defPosX - 1, defPosY))
            {
                count++;
            }
            return 10 + 60 * count;
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            break;
        }
    }
    return 0;
};
CO_XAVIER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_XAVIER.getBonusLuck = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (hpRounded <= 5)
            {
                return hpRounded / 2;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (hpRounded <= 5)
            {
                return 20 + hpRounded / 2;
            }
            return 20;
        default:
            if (hpRounded <= 5)
            {
                return hpRounded / 2;
            }
        }
    }
    return 0;
};

CO_XAVIER.getBonusMisfortune = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (hpRounded <= 5)
            {
                return -hpRounded;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (hpRounded <= 5)
            {
                return -20 + -hpRounded;
            }
            return -20;
        default:
            if (hpRounded <= 5)
            {
                return -hpRounded;
            }
            break;
        }
    }
    return 0;
};
