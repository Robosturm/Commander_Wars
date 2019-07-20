var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Tank");
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
            case "RUIN":
            case "STREET":
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
            case "TEMPORARY_AIRPORT":
            case "TEMPORARY_HARBOUR":
            case "LABOR":
            case "DESERT":
            case "DESERT_DESTROYEDWELD":
            case "DESERT_PATH":
                return 1;
			case "FOREST":
			case "WASTELAND":
            case "DESERT_TRY_RIVER":
            case "SNOW":
            case "SNOW_DESTROYEDWELD":
            case "DESERT_FOREST":
            case "DESERT_WASTELAND":
                return 2;
            case "SNOW_FOREST":
            case "SNOW_WASTELAND":
                return 3;
        }
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_TANK = new Constructor();
