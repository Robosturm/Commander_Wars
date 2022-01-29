CO_LASH.globalTerrainBonus = 10;
CO_LASH.zoneTerrainBonus = 0;
CO_LASH.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
    {
        if (map !== null)
        {
            if (map.onMap(atkPosX, atkPosY))
            {
                if (attacker.useTerrainDefense())
                {
                    var terrainDefense = map.getTerrain(atkPosX, atkPosY).getDefense(attacker);
                    return terrainDefense * CO_LASH.globalTerrainBonus;
                }
                else if (co.getPowerMode() > GameEnums.PowerMode_Off)
                {
                    return 10;
                }
            }
        }
    }
    return 0;
};

CO_LASH.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_LASH.getTerrainDefenseModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return map.getTerrain(posX, posY).getBaseDefense();
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            return 0;
        }
    }
    return 0;
};
CO_LASH.getMovementcostModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return -999;
            default:
                return 0;
            }
        }
    }
    return 0;
};
