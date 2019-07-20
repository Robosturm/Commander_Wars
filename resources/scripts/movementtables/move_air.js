var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Air");
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
            case "MINE":
            case "PIPESTATION":
            case "RADAR":
            case "TOWER":
            case "TOWN":
            case "SILO":
            case "SILO_ROCKET":
            case "LABOR":
            case "TEMPORARY_AIRPORT":
            case "TEMPORARY_HARBOUR":
			case "FOREST":
			case "WASTELAND":
            case "SEA":
			case "ROUGH_SEA":
            case "REAF":
            case "MOUNTAIN":
            case "RIVER":
            case "DESERT":
            case "DESERT_DESTROYEDWELD":
            case "DESERT_FOREST":
            case "DESERT_PATH":
            case "DESERT_ROCK":
            case "DESERT_TRY_RIVER":
            case "DESERT_WASTELAND":
            case "SNOW":
            case "SNOW_DESTROYEDWELD":
            case "SNOW_FOREST":
            case "SNOW_MOUNTAIN":
            case "SNOW_WASTELAND":
				return 1;
        }
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_AIR = new Constructor();
