CO_VON_BOLT.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_VON_BOLT.activateSuperpower = function(co, powerMode, map)
{
    CO_VON_BOLT.activatePower(co, powerMode);
};
CO_VON_BOLT.getSuperPowerDescription = function()
{
    return CO_VON_BOLT.getPowerDescription();
};
CO_VON_BOLT.getSuperPowerName = function()
{
    return CO_VON_BOLT.getPowerName();
};
CO_VON_BOLT.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
    {
        return 10;
    }
    return 0;
};
CO_VON_BOLT.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isAttacker, action, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            return 10;
        }
    }
    return 0;
};
