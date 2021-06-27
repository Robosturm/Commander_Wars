CO_FORSYTHE.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_FORSYTHE.activateSuperpower = function(co, powerMode)
{
    CO_FORSYTHE.activatePower(co, powerMode);
};
CO_FORSYTHE.getSuperPowerDescription = function()
{
    return CO_FORSYTHE.getPowerDescription();
};
CO_FORSYTHE.getSuperPowerName = function()
{
    return CO_FORSYTHE.getPowerName();
};
CO_FORSYTHE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender, action)
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

CO_FORSYTHE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                   defender, defPosX, defPosY, isDefender, action)
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
    return 0;
};
