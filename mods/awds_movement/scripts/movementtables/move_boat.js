MOVE_BOAT.getMovementpoints = function(terrain, unit, currentTerrain)
{
    return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_BOAT.movementpointsTable);
};
