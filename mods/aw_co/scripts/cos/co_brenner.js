CO_BRENNER.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_BRENNER.activateSuperpower = function(co, powerMode)
{
    CO_BRENNER.activatePower(co, powerMode);
};
CO_BRENNER.getSuperPowerDescription = function()
{
    return CO_BRENNER.getPowerDescription();
};
CO_BRENNER.getSuperPowerName = function()
{
    return CO_BRENNER.getPowerName();
};
CO_BRENNER.coZoneBonus = 0;
CO_BRENNER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
            return 0;
        }
    }
    return 0;
};

CO_BRENNER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};
