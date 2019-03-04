var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Tire");
    };
    this.getMovementpoints = function(terrainID)
    {
        switch (terrainID)
        {   
			case "RUIN":	
			case "PLAINS":
            case "BEACH":
			case "DESTROYEDWELD":
				return 2;
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
                return 1;
			case "FOREST":
			case "WASTELAND":
				return 3;
        }
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_TIRE_A = new Constructor();
