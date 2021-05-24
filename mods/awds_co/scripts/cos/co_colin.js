CO_COLIN.globalBoost = 10;
CO_COLIN.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            var bonus = attacker.getOwner().getFunds() / 1000 * 3.33;
            return bonus;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            return -CO_COLIN.globalBoost;
        }
    }
    return 0;
};

CO_COLIN.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
this.getCostModifier = function(co, id, baseCost)
{
    if (co.getIsCO0() === true)
    {
        return -baseCost * CO_COLIN.costModifier / 100;
    }
    return 0;
};
