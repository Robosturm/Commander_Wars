CO_KANBEI.getCostModifier = function(co, id, baseCost)
{
    if (co.getIsCO0() === true)
    {
        return baseCost * CO_KANBEI.costIncrease / 100;
    }
    return 0;
};
CO_KANBEI.coZoneBonus = 0;
CO_KANBEI.globalBonus = 30;
CO_KANBEI.costIncrease = 20;
CO_KANBEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
                return 100;
            }
            return 50;
        case GameEnums.PowerMode_Power:
            return 50;
        default:
            return CO_KANBEI.globalBonus;
        }
    }
    return 0;
};
CO_KANBEI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 60;
        case GameEnums.PowerMode_Power:
            return 40;
        default:
            return CO_KANBEI.globalBonus;
        }
    }
    return 0;
};
