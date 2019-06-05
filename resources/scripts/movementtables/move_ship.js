var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Ship");
    };
    this.getMovementpoints = function(terrain)
    {
        switch (terrain.getID())
        {
            case "HARBOUR":
            case "TEMPORARY_HARBOUR":
            case "SEA":
            case "FOG":
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
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_SHIP = new Constructor();
