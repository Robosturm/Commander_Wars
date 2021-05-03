var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Hovercraft");
    };
    this.movementpointsTable = [["PLAINS", 1],
                                ["PLAINS_DESTROYED", 1],
                                ["PLAINS_PLASMA", 1],
                                ["BEACH", 1],
                                ["BRIDGE", 1],
                                ["DESTROYEDWELD", 1],
                                ["RUIN", 1],
                                ["STREET", 1],
                                ["AIRPORT", 1],
                                ["FACTORY", 1],
                                ["HARBOUR", 1],
                                ["HQ", 1],
                                ["PIPESTATION", 1],
                                ["RADAR", 1],
                                ["TOWER", 1],
                                ["TOWN", 1],
                                ["SILO", 1],
                                ["SILO_ROCKET", 1],
                                ["LABOR", 1],
                                ["TEMPORARY_AIRPORT", 1],
                                ["TEMPORARY_HARBOUR", 1],
                                ["OILRIG", 1],
                                ["RIVER", 1],
                                ["DESERT", 1],
                                ["DESERT_DESTROYEDWELD", 1],
                                ["DESERT_PATH", 1],
                                ["DESERT_TRY_RIVER", 1],
                                ["SNOW", 1],
                                ["SNOW_DESTROYEDWELD", 1],
                                ["FOREST", 2],
                                ["WASTELAND", 2],
                                ["DESERT_FOREST", 2],
                                ["DESERT_WASTELAND", 2],
                                ["SNOW_FOREST", 2],
                                ["SNOW_WASTELAND", 2],
                                ["SEA", 2],
                                ["FOG", 2],
                                ["ROUGH_SEA", 3],
                                ["REAF", 3],
                                ["TELEPORTTILE", 0]];

    this.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false)
    {
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
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_HOVERCRAFT = new Constructor();
