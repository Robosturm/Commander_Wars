MOVE_AIR.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false)
{
    var id = terrain.getID();
    var currentUnit = terrain.getUnit();
    if ((currentUnit !== null) &&
        (unit.getOwner().isEnemy(currentUnit.getOwner())))
    {
        if (!currentUnit.isStealthed(unit.getOwner()) || trapChecking)
        {
            if (currentUnit.getUnitType() === GameEnums.UnitType_Air)
            {
                return -1;
            }
        }
    }
    if ((id === "ZGATE_E_W" || id === "ZGATE_N_S") &&
            (unit !== null) &&
            (unit.getOwner().isAlly(terrain.getBuilding().getOwner())))
    {
        return 1;
    }
    return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_AIR.movementpointsTable);
};
