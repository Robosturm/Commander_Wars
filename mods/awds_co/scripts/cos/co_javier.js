CO_JAVIER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        var towers = co.getOwner().getBuildingCount("TOWER");
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return towers * 25 + 10;
        case GameEnums.PowerMode_Power:
            return towers * 15 + 10;
        default:
            return towers * 5 + 10;
        }
    }
    return 0;
};
CO_JAVIER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        var rangedAttacked = (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1);
        var towers = co.getOwner().getBuildingCount("TOWER");
        var ret = 0;
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (rangedAttacked)
            {
                ret += 60;
            }
            ret += towers * 5 + 10;
            break;
        case GameEnums.PowerMode_Power:
            if (rangedAttacked)
            {
                ret += 40;
            }
            ret += towers * 5 + 10;
            break;
        default:
            if (rangedAttacked)
            {
                ret += 20;
            }
            ret += towers * 5;
            break;
        }
        return ret;
    }
    return 0;
};
