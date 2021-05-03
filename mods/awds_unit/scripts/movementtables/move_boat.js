var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Boat");
    };
    this.movementpointsTable = [["BEACH", 1],
                                ["HARBOUR", 1],
                                ["OILRIG", 1],
                                ["TEMPORARY_HARBOUR", 1],
                                ["FOG", 1],
                                ["SEA", 1],
                                ["ROUGH_SEA", 2],
                                ["REAF", 2],
                                ["TELEPORTTILE", 0]];

    this.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking)
    {
        if (terrain.getID() === "BRIDGE" && terrain.getBaseTerrainID() === "SEA")
        {
            return 1;
        }
        return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_BOAT.movementpointsTable);
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_BOAT = new Constructor();
