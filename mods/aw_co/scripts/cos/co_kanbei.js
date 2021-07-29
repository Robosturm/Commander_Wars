CO_KANBEI.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_KANBEI.activateSuperpower = function(co, powerMode)
{
    CO_KANBEI.activatePower(co, powerMode);
};
CO_KANBEI.getSuperPowerDescription = function()
{
    return CO_KANBEI.getPowerDescription();
};
CO_KANBEI.getSuperPowerName = function()
{
    return CO_KANBEI.getPowerName();
};
CO_KANBEI.getCostModifier = function(co, id, baseCost)
{
    if (co.getIsCO0() === true)
    {
        return baseCost * CO_KANBEI.costIncrease / 100;
    }
    return 0;
};
CO_KANBEI.coZoneBonus = 0;
CO_KANBEI.globalBonus = 20;
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
        case GameEnums.PowerMode_Power:
            return 50;
        default:
            return CO_KANBEI.globalBonus;
        }
    }
    return 0;
};
CO_KANBEI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isAttacker, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 40;
        default:
            return CO_KANBEI.globalBonus;
        }
    }
    return 0;
};
