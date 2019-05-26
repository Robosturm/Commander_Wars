CO_MELANTHE.getTerrainDefenseModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (CO_MELANTHE.isNature(posX, posY) === true)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 2;
            case GameEnums.PowerMode_Power:
                return 2;
            default:
                return 1;
            }
        }
    }
    return 0;
};
CO_MELANTHE.getRepairBonus = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        return -1;
    }
    return 0;
};
CO_MELANTHE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        if (map.onMap(atkPosX, atkPosY))
        {
            if (CO_MELANTHE.isNature(atkPosX, atkPosY) === true)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return 20;
                case GameEnums.PowerMode_Power:
                    return 20;
                default:
                    return 15;
                }
            }
        }
    }
    return 0;
};
