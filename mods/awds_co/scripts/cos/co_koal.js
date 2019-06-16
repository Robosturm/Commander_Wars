CO_KOAL.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        if (map.onMap(atkPosX, atkPosY))
        {
            var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (terrainID === "STREET")
                {
                    return 40;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (terrainID === "STREET")
                {
                    return 30;
                }
                break;
            default:
                if (terrainID === "STREET")
                {
                    return 10;
                }
                break;
            }
        }
    }
    return 0;
};

CO_KOAL.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
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

CO_KOAL.getMovementpointModifier = function(co, unit, posX, posY)
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
