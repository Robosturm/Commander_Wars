CO_WALTER.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_WALTER.activateSuperpower = function(co, powerMode)
{
    CO_WALTER.activatePower(co, powerMode);
};
CO_WALTER.getSuperPowerDescription = function()
{
    return CO_WALTER.getPowerDescription();
};
CO_WALTER.getSuperPowerName = function()
{
    return CO_WALTER.getPowerName();
};
CO_WALTER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isAttacker, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 10;
        default:
        }
    }
    return 0;
};

CO_WALTER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};
