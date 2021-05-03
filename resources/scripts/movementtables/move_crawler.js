var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Crawler");
    };
    this.movementpointsTable = [["PLAINS", 1],
                                ["PLAINS_DESTROYED", 1],
                                ["PLAINS_PLASMA", 1],
                                ["BEACH", 1],
                                ["BRIDGE", 1],
                                ["DESTROYEDWELD", 1],
                                ["FOREST", 1],
                                ["RUIN", 1],
								["WASTELAND", 1],
                                ["STREET", 1],
								["MARSHLAND", 1],
                                ["AIRPORT", 1],
                                ["FACTORY", 1],
                                ["HARBOUR", 1],
                                ["HQ", 1],
                                ["MINE", 2],
                                ["PIPESTATION", 1],
                                ["RADAR", 1],
                                ["TOWER", 1],
                                ["TOWN", 1],
                                ["SILO", 1],
                                ["SILO_ROCKET", 1],
                                ["LABOR", 1],
                                ["TEMPORARY_AIRPORT", 1],
								["TEMPORARY_AIRPORT_DESERT", 1],
								["TEMPORARY_AIRPORT_SNOW", 1],
                                ["TEMPORARY_HARBOUR", 1],
                                ["MOUNTAIN", 2],
                                ["RIVER", 2],
                                ["DESERT", 1],
                                ["DESERT_DESTROYEDWELD", 1],
                                ["DESERT_PATH", 1],
                                ["DESERT_TRY_RIVER", 1],
								["DESERT_ROCK", 2],
                                ["DESERT_FOREST", 1],
                                ["DESERT_WASTELAND", 1],
                                ["SNOW", 2],
                                ["SNOW_DESTROYEDWELD", 2],
								["SNOW_MOUNTAIN", 3],                                
                                ["SNOW_FOREST", 2],
                                ["SNOW_WASTELAND", 2],
                                ["TELEPORTTILE", 0]];

    this.getMovementpoints = function(terrain, unit, trapChecking = false)
    {
        var id = terrain.getID();
        if ((id === "ZGATE_E_W" || id === "ZGATE_N_S") &&
            (unit !== null) &&
            (unit.getOwner().isAlly(terrain.getBuilding().getOwner())))
        {
            return 1;
        }
        return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_CRAWLER.movementpointsTable);
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_CRAWLER = new Constructor();
