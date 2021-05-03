MOVE_SHIP.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking)
{
    return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_SHIP.movementpointsTable);
};
