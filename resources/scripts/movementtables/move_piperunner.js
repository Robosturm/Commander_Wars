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
                                ["ZGATE_E_W", 1],
                                ["ZGATE_N_S", 1],
                                ["DESERT_PIPELINE", 1],
                                ["SNOW_WELD", 2],
                                ["SNOW_PIPELINE", 2]];

    this.getMovementpoints = function(terrain, unit)
    {
        return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_PIPERUNNER.movementpointsTable);
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_PIPERUNNER = new Constructor();
