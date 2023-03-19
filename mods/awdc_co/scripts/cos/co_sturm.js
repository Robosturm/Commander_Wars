CO_STURM.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_STURM.activateSuperpower = function(co, powerMode, map)
{
	CO_STURM.activatePower(co, map);
};
CO_STURM.getSuperPowerDescription = function()
{
    return CO_STURM.getPowerDescription();
};
CO_STURM.getSuperPowerName = function()
{
    return CO_STURM.getPowerName();
};
CO_STURM.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 30;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 30;
            }
            break;
    }
    return 0;
};
CO_STURM.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 30;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return 30;
            }
            break;
    }
    return 0;
};
