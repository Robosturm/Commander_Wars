var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Mech");
    };
    this.getMovementpoints = function(terrainID)
    {
        switch (terrainID)
        {
            case "PLAINS":
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
            case "MINE":
            case "PIPESTATION":
            case "RADAR":
            case "TOWER":
            case "TOWN":
			case "MOUNTAIN":
            case "RIVER":
                return 1;
        }
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_MECH = new Constructor();
