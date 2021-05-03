MOVE_BOAT.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking)
{
    return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_BOAT.movementpointsTable);
};
