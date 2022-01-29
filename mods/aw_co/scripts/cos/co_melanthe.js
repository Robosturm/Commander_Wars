CO_MELANTHE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_MELANTHE.activateSuperpower = function(co, powerMode, map)
{
    CO_MELANTHE.activatePower(co, powerMode);
};
CO_MELANTHE.getSuperPowerDescription = function()
{
    return CO_MELANTHE.getPowerDescription();
};
CO_MELANTHE.getSuperPowerName = function()
{
    return CO_MELANTHE.getPowerName();
};
CO_MELANTHE.getTerrainDefenseModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (CO_MELANTHE.isNature(posX, posY, map) === true)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return 2;
            default:
                return 1;
            }
        }
    }
    return 0;
};
CO_MELANTHE.getRepairBonus = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        return -1;
    }
    return 0;
};
CO_MELANTHE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
    {
        if (map !== null)
        {
            if (map.onMap(atkPosX, atkPosY))
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                case GameEnums.PowerMode_Power:
                    if (CO_MELANTHE.isNature(atkPosX, atkPosY) === true)
                    {
                        return 20;
                    }
                    return 0;
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
                                       defender, defPosX, defPosY, isAttacker, action, map)
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
