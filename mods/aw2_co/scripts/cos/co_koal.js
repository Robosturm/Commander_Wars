CO_KOAL.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        if (map !== null)
        {
            if (map.onMap(atkPosX, atkPosY))
            {
                var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
                var isStreet = (terrainID === "STREET") ||
                               (terrainID === "STREET1") ||
                               (terrainID === "SNOW_STREET") ||
                               (terrainID === "BRIDGE") ||
                               (terrainID === "BRIDGE1") ||
                               (terrainID === "BRIDGE2") ||
                               (terrainID === "WASTE_PATH") ||
                               (terrainID === "DESERT_PATH") ||
                               (terrainID === "DESERT_PATH1");
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (isStreet)
                    {
                        return 30;
                    }
                    return 0;
                case GameEnums.PowerMode_Power:
                    if (isStreet)
                    {
                        return 20;
                    }
                    return 0;
                default:
                    if (isStreet)
                    {
                        return 10;
                    }
                    return 0;
                }
            }
        }
    }
    return 0;
};

CO_KOAL.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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

CO_KOAL.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 2;
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
    }
    return 0;
};
