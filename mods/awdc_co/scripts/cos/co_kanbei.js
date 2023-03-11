CO_KANBEI.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_KANBEI.activateSuperpower = function(co, powerMode, map)
{
	CO_KANBEI.activatePower(co, map);
};
CO_KANBEI.getSuperPowerDescription = function()
{
    return CO_KANBEI.getPowerDescription();
};
CO_KANBEI.getSuperPowerName = function()
{
    return CO_KANBEI.getPowerName();
};
CO_KANBEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        return CO_KANBEI.powerAtkBonus;
    default:
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return CO_KANBEI.coZoneBonus;
        }
        break;
    }
    return CO_KANBEI.globalBonus;
};
CO_KANBEI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                   defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        return CO_KANBEI.powerDefBonus;
    default:
        if (co.inCORange(Qt.point(defPosX, defPosY), defender))
        {
            return CO_KANBEI.coZoneBonus;
        }
        break;
    }
    return CO_KANBEI.globalBonus;
};
