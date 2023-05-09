var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Small boat");
    };
    this.movementpointsTable = [["BEACH", 1],
                                ["HARBOUR", 1],
                                ["TEMPORARY_HARBOUR", 1],
                                ["FOG", 1],
                                ["SEA", 1],
                                ["LAKE", 1],
                                ["RIVER", 1],
                                ["ROUGH_SEA", 2],
                                ["REAF", 2],
                                ["TELEPORTTILE", 0],
                                ["OILRIG", 1],];

    this.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false, map)
    {
        let terrainId = terrain.getTerrainID();
        let id = terrain.getID();
        let shipBridges = true;
        if (map !== null)
        {
            shipBridges = map.getGameRules().getShipBridges();
        }
        if (shipBridges &&
            id === "BRIDGE" &&
            (terrain.getBaseTerrainID() === "SEA" ||
             terrain.getBaseTerrainID() === "LAKE" ||
             terrain.getBaseTerrain("RIVER", true) !== null))
        {
            return 1;
        }
        else if (id === "OILRIG")
        {
            if (terrainId === "SEA" ||
                terrainId === "LAKE")
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
        return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_SMALL_BOAT.movementpointsTable);
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_SMALL_BOAT = new Constructor();
