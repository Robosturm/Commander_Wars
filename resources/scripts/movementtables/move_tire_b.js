var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Tire B");
    };
    this.movementpointsTable = [["DESERT_FOREST", 2],
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
            ["DESERT", 1],
            ["DESERT_DESTROYEDWELD", 1],
            ["DESERT_PATH", 1],
			["FOREST", 3],
			["WASTELAND", 3],
            ["DESERT_TRY_RIVER", 3],
            ["SNOW_FOREST", 4],
            ["SNOW_WASTELAND", 4],
            ["TELEPORTTILE", 0]];

    this.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false)
    {
        var id = terrain.getID();
        if ((id === "ZGATE_E_W" || id === "ZGATE_N_S") &&
            (unit !== null) &&
            (unit.getOwner().isAlly(terrain.getBuilding().getOwner())))
        {
            return 1;
        }
        return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_TIRE_B.movementpointsTable);
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_TIRE_B = new Constructor();
