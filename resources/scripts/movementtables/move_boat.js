var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Boat");
    };
    this.getMovementpoints = function(terrain)
    {
        switch (terrain.getID)
        {
			case "BEACH":
            case "HARBOUR":
            case "FOG":
            case "SEA":
                return 1;
			case "ROUGH_SEA":
            case "REAF":
				return 2;
            case "BRIDGE":
                if (terrain.getBaseTerrainID() === "SEA")
                {
                    return 1;
                }
                break;
        }
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_BOAT = new Constructor();
