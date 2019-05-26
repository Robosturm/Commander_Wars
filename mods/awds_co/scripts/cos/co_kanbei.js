CO_KANBEI.getCostModifier = function(co, id, baseCost)
{
    if (co.getIsCO0() === true)
    {
        return baseCost * 0.2;
    }
    return 0;
};
CO_KANBEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (isDefender)
            {
                return 140;
            }
            return 70;
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            return 30;
        }
    }
    return 0;
};
CO_KANBEI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 70;
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            return 30;
        }
    }
    return 0;
};
