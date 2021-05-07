MOVE_HOVERCRAFT.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false)
{
    var currentUnit = terrain.getUnit();
    if ((currentUnit !== null) &&
        (unit.getOwner().isEnemy(currentUnit.getOwner())))
    {
        if (!currentUnit.isStealthed(unit.getOwner()) || trapChecking)
        {
            if (currentUnit.getUnitType() !== GameEnums.UnitType_Air)
            {
                return -1;
            }
        }
    }
    var id = terrain.getID();
    var currentId = currentTerrain.getID();
    if ((id === "ZGATE_E_W" || id === "ZGATE_N_S") &&
            (unit !== null) &&
            (unit.getOwner().isAlly(terrain.getBuilding().getOwner())))
    {
        return 1;
    }
    var costs = MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_HOVERCRAFT.movementpointsTable);
    var currentGroup = currentTerrain.getTerrainGroup();
    var targetGroup = terrain.getTerrainGroup();
    if (currentGroup === targetGroup)
    {
        return costs;
    }
    else
    {
        // from sea to land or vice versa
        if (currentGroup === 0 || targetGroup === 0)
        {
            // fields we can move from land to sea
            var crossable = ["HARBOUR", "BEACH", "TEMPORARY_HARBOUR", "RIVER", "TELEPORTTILE"];
            for (var i = 0; i < crossable.length; i++)
            {
                if (crossable[i] === id ||
                        crossable[i] === currentId)
                {
                    return costs;
                }
            }
        }
        else
        {
            return costs;
        }
    }
    return -1;
};
