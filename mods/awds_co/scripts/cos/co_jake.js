
CO_JAKE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
                if (terrainID === "PLAINS")
                {
                    return 40;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (terrainID === "PLAINS")
                {
                    return 20;
                }
                break;
            default:
                if (terrainID === "PLAINS")
                {
                    return 10;
                }
                break;
            }
        }
    }
    return 0;
};
CO_JAKE.getFirerangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var indirectUnits = CO_JAKE.getGroundIndirectUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (indirectUnits.indexOf(unit.getUnitID()) >= 0)
            {
                return 1;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (indirectUnits.indexOf(unit.getUnitID()) >= 0)
            {
                return 1;
            }
            break;
        default:
            return 0;
        }
    }
    return 0;
};
CO_JAKE.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var vehicleUnits = CO_JAKE.getVehicleUnitIDS();
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if (vehicleUnits.indexOf(unit.getUnitID()) >= 0)
            {
                return 2;
            }
        }
    }
    return 0;
};
