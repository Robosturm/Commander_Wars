MOVE_SHIP.getMovementpoints = function(terrain, unit, currentTerrain)
{
    return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_SHIP.movementpointsTable);
};
