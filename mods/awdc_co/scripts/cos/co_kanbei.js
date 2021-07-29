CO_KANBEI.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
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
CO_KANBEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender, action)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        return 60;
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
                                   defender, defPosX, defPosY, isAttacker, action)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        return 60;
    default:
        if (co.inCORange(Qt.point(defPosX, defPosY), defender))
        {
            return CO_KANBEI.coZoneBonus;
        }
        break;
    }
    return CO_KANBEI.globalBonus;
};
