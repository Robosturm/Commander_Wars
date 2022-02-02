CO_ROBOSTURM.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_ROBOSTURM.activateSuperpower = function(co, powerMode, map)
{
    CO_ROBOSTURM.activatePower(co, powerMode);
};
CO_ROBOSTURM.getSuperPowerDescription = function()
{
    return CO_ROBOSTURM.getPowerDescription();
};
CO_ROBOSTURM.getSuperPowerName = function()
{
    return CO_ROBOSTURM.getPowerName();
};
CO_ROBOSTURM.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            break;
        }
        return -20;
    }
    return 0;
};
CO_ROBOSTURM.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            return 20;
        }
    }
};
CO_ROBOSTURM.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 2;
        }
        return 1;
    }
    return 0;
};
