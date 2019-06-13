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
			case "RUIN":	
			case "PLAINS":
            case "PLAINS_DESTROYED":
            case "PLAINS_PLASMA":
            case "BEACH":
			case "DESTROYEDWELD":
				return 2;
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
                return 1;
			case "FOREST":
			case "WASTELAND":
				return 3;
        }
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_TIRE_A = new Constructor();
