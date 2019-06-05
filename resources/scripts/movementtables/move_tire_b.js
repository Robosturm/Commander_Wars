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
            case "BEACH":
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
                return 1;
			case "FOREST":
			case "WASTELAND":
				return 3;
        }
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_TIRE_B = new Constructor();
