CO_JAVIER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_JAVIER.activateSuperpower = function(co, powerMode, map)
{
    CO_JAVIER.activatePower(co, map);
};
CO_JAVIER.getSuperPowerDescription = function()
{
    return CO_JAVIER.getPowerDescription();
};
CO_JAVIER.getSuperPowerName = function()
{
    return CO_JAVIER.getPowerName();
};
CO_JAVIER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        var towers = co.getOwner().getBuildingCount("TOWER");
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return towers * 15;
        default:
            return towers * 5;
        }
    }
    return 0;
};
CO_JAVIER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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
