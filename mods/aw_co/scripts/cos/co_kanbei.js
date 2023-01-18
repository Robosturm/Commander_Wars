CO_KANBEI.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_KANBEI.activateSuperpower = function(co, powerMode, map)
{
    CO_KANBEI.activatePower(co, powerMode, map);
};
CO_KANBEI.getSuperPowerDescription = function()
{
    return CO_KANBEI.getPowerDescription();
};
CO_KANBEI.getSuperPowerName = function()
{
    return CO_KANBEI.getPowerName();
};
CO_KANBEI.getCostModifier = function(co, id, baseCost, posX, posY, map)
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
CO_KANBEI.powerAtkBonus = 50;
CO_KANBEI.superPowerAtkBonus = 50;
CO_KANBEI.superPowerCounterBonus = 100;
CO_KANBEI.powerDefBonus = 40;
CO_KANBEI.superPowerDefBonus = 60;
CO_KANBEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return CO_KANBEI.powerAtkBonus;
        default:
            return CO_KANBEI.globalBonus;
        }
    }
    return 0;
};
CO_KANBEI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return CO_KANBEI.powerDefBonus;
        default:
            return CO_KANBEI.globalBonus;
        }
    }
    return 0;
};
