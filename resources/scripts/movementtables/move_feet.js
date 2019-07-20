var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Feet");
    };
    this.getMovementpoints = function(terrain)
    {
        switch (terrain.getID())
        {
            case "PLAINS":
            case "PLAINS_DESTROYED":
            case "PLAINS_PLASMA":
            case "BEACH":
            case "BRIDGE":
            case "DESTROYEDWELD":
            case "FOREST":
            case "RUIN":
            case "STREET":
            case "WASTELAND":
            case "AIRPORT":
            case "FACTORY":
            case "HARBOUR":
            case "HQ":
            case "PIPESTATION":
            case "RADAR":
            case "TOWER":
            case "TOWN":
            case "SILO":
            case "SILO_ROCKET":
            case "LABOR":
            case "TEMPORARY_AIRPORT":
            case "TEMPORARY_HARBOUR":

            case "DESERT_WASTELAND":
            case "DESERT_PATH":
            case "DESERT":
            case "DESERT_DESTROYEDWELD":
            case "DESERT_FOREST":
                return 1;
            case "MINE":
            case "MOUNTAIN":
            case "DESERT_ROCK":
            case "DESERT_TRY_RIVER":
            case "RIVER":
            case "SNOW":
            case "SNOW_DESTROYEDWELD":
            case "SNOW_FOREST":
            case "SNOW_WASTELAND":
                return 2;
            case "SNOW_MOUNTAIN":
                return 3;
        }
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_FEET = new Constructor();
