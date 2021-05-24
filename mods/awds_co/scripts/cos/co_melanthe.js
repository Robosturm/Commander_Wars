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
                                         defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (typeof map !== 'undefined')
        {
            if (map.onMap(atkPosX, atkPosY))
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (CO_MELANTHE.isNature(atkPosX, atkPosY) === true)
                    {
                        return 30;
                    }
                    return 10;
                case GameEnums.PowerMode_Power:
                    if (CO_MELANTHE.isNature(atkPosX, atkPosY) === true)
                    {
                        return 30;
                    }
                    return 10;
                default:
                    if (CO_MELANTHE.isNature(atkPosX, atkPosY) === true)
                    {
                        return 15;
                    }
                }
            }
        }
    }
    return 0;
};

CO_MELANTHE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
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
