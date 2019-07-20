var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Tire");
    };
    this.getMovementpoints = function(terrain)
    {
        switch (terrain.getID())
        {
            case "DESERT_FOREST":
            case "DESERT_WASTELAND":
            case "BEACH":
            case "SNOW":
            case "SNOW_DESTROYEDWELD":
				return 2;            
            case "RUIN":			
			case "DESTROYEDWELD":
			case "PLAINS":
            case "PLAINS_DESTROYED":
            case "PLAINS_PLASMA":
			case "BRIDGE":
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
            case "LABOR":
            case "TEMPORARY_AIRPORT":
            case "TEMPORARY_HARBOUR":
            case "DESERT":
            case "DESERT_DESTROYEDWELD":
            case "DESERT_PATH":
                return 1;
			case "FOREST":
			case "WASTELAND":
            case "DESERT_TRY_RIVER":
                return 3;
            case "SNOW_FOREST":
            case "SNOW_WASTELAND":
                return 4;
        }
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_TIRE_B = new Constructor();
