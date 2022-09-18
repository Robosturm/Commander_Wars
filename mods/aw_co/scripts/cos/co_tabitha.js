CO_TABITHA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_TABITHA.activateSuperpower = function(co, powerMode, map)
{
    CO_TABITHA.activatePower(co, powerMode, map);
};
CO_TABITHA.getSuperPowerDescription = function()
{
    return CO_TABITHA.getPowerDescription();
};
CO_TABITHA.getSuperPowerName = function()
{
    return CO_TABITHA.getPowerName();
};
CO_TABITHA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender, action, luckmode, map)
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
            break;
        }
    }
    return 10;
};
CO_TABITHA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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
            break;
        }
    }
    return 10;
};
