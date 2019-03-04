var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Hovercraft");
    };
    this.getMovementpoints = function(terrainID)
    {
        switch (terrainID)
        {
            case "PLAINS":
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
			case "RIVER":
                return 1;
			case "FOREST":
			case "WASTELAND":
				return 2;
        }
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_HOVERCRAFT = new Constructor();
