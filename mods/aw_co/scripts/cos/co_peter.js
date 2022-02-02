CO_PETER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_PETER.activateSuperpower = function(co, powerMode, map)
{
    CO_PETER.activatePower(co, powerMode);
};
CO_PETER.getSuperPowerDescription = function()
{
    return CO_PETER.getPowerDescription();
};
CO_PETER.getSuperPowerName = function()
{
    return CO_PETER.getPowerName();
};
CO_PETER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        if (defender === null)
        {
            return 0;
        }
        var bonus = 0;
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (defender.getBaseMaxRange() === 1)
            {
                bonus += 60;
            }
            break;
        default:
            bonus = 20;
            break;
        }
        bonus -= defender.getTerrainDefense() * 5;
        return bonus;
    }
    return 0;
};
CO_PETER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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
