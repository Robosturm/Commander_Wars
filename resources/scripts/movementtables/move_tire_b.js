var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Tire B");
    };
    this.movementpointsTable = [["DESERT_FOREST", 2],
                                ["DESERT_FOREST1", 2],
                                ["DESERT_WASTELAND", 2],
                                ["BEACH", 2],
                                ["SNOW", 2],
                                ["SNOW_DESTROYEDWELD", 2],
                                ["RUIN", 1],
                                ["DESTROYEDWELD", 1],
                                ["PLAINS", 1],
                                ["PLAINS_DESTROYED", 1],
                                ["PLAINS_PLASMA", 1],
                                ["BRIDGE", 1],
                                ["BRIDGE1", 1],
                                ["BRIDGE2", 1],
                                ["STREET", 1],
                                ["SNOW_STREET", 1],
                                ["STREET1", 1],
                                ["AIRPORT", 1],
                                ["FACTORY", 1],
                                ["HARBOUR", 1],
                                ["HQ", 1],
                                ["PIPESTATION", 1],
                                ["POWERPLANT", 1],
                                ["AMPHIBIOUSFACTORY", 1],
                                ["RADAR", 1],
                                ["TOWER", 1],
                                ["TOWN", 1],
                                ["SILO", 1],
                                ["DEPOT", 1],
                                ["SILO_ROCKET", 1],
                                ["LABOR", 1],
                                ["TEMPORARY_AIRPORT", 1],
                                ["TEMPORARY_HARBOUR", 1],
                                ["ZDESTROYED_GATE_E_W", 1],
                                ["ZDESTROYED_GATE_N_S", 1],
                                ["DESERT", 1],
                                ["DESERT_DESTROYEDWELD", 1],
                                ["DESERT_PATH", 1],
                                ["DESERT_PATH1", 1],
                                ["DESERT_RUIN", 1],
                                ["FOREST", 3],
                                ["FOREST1", 3],
                                ["FOREST2", 3],
                                ["FOREST3", 3],
                                ["WASTELAND", 3],
                                ["DESERT_TRY_RIVER", 3],
                                ["SNOW_FOREST", 4],
                                ["SNOW_FOREST1", 4],
                                ["SNOW_FOREST2", 4],
                                ["SNOW_WASTELAND", 4],
                                ["SNOW_RUIN", 2],
                                ["WASTE",  1],
                                ["WASTE_PATH", 1],
                                ["WASTE_FOREST", 3],
                                ["WASTE_WASTELAND", 3],
                                ["WASTE_RUIN", 1],
                                ["WASTE_DESTROYEDWELD", 1],
                                ["CREEPER", 1],
                                ["TELEPORTTILE", 0],
                                ["ZGATE_E_W", 1],
                                ["ZGATE_N_S", 1],
                                ["FORTHQ", 1],
                                ["OILRIG", 1],];

    this.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false, map)
    {
        var terrainId = terrain.getTerrainID();
        var id = terrain.getID();
        if (id === "ZGATE_E_W" || id === "ZGATE_N_S" || id === "FORTHQ")
        {
            if ((unit !== null) && (unit.getOwner().isAlly(terrain.getBuilding().getOwner())))
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
        else if (id === "OILRIG")
        {
            if (terrainId !== "SEA" &&
                terrainId !== "LAKE")
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
        return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_TIRE_B.movementpointsTable);
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_TIRE_B = new Constructor();
