CO_PETER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        if (defender === null)
        {
            return 0;
        }
        var bonus = 0;
        if (defender.getBaseMaxRange() === 1)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 40;
            case GameEnums.PowerMode_Power:
                return 60;
            default:
                bonus = 30;
                break;
            }
        }
        bonus -= defender.getTerrainDefense() * 5;
        return bonus;
    }
    return 0;
};
