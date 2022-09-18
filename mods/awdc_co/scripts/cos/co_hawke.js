CO_HAWKE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_HAWKE.activateSuperpower = function(co, powerMode, map)
{
	CO_HAWKE.activatePower(co, powerMode, map);
};
CO_HAWKE.getSuperPowerDescription = function()
{
    return CO_HAWKE.getPowerDescription();
};
CO_HAWKE.getSuperPowerName = function()
{
    return CO_HAWKE.getPowerName();
};
CO_HAWKE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 40;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 40;
            }
            break;
    }
    return 0;
};
CO_HAWKE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                   defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
        co.getPowerMode() > GameEnums.PowerMode_Off)
    {
        return 20;
    }
    return 0;
};
