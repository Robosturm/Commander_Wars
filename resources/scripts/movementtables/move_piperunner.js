var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Piperunner");
    };
    this.movementpointsTable = [["WELD", 1],
                                ["FACTORY", 1],
                                ["PIPELINE", 1],
                                ["PIPESTATION", 1],
                                ["DESERT_WELD", 1],
                                ["DESERT_PIPELINE", 1],
                                ["SNOW_WELD", 2],
                                ["SNOW_PIPELINE", 2],
                                ["ZWELD_N_S", 1],
                                ["ZWELD_E_W", 1],
                                ["ZSNOWWELD_N_S", 2],
                                ["ZSNOWWELD_E_W", 2],
                                ["ZDESERTWELD_N_S", 1],
                                ["ZDESERTWELD_E_W", 1],
                                ["TELEPORTTILE", 0]];

    this.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false)
    {
        return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_PIPERUNNER.movementpointsTable);
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_PIPERUNNER = new Constructor();
