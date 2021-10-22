CO_BRENNER.coZoneBonus = 0;
CO_BRENNER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isAttacker, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return CO_BRENNER.superPowerBonus;
        case GameEnums.PowerMode_Power:
            return CO_BRENNER.powerBonus;
        default:
            return 10;
        }
    }
    return 0;
};

CO_BRENNER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};
