var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Boat");
    };
    this.getMovementpoints = function(terrainID)
    {
        switch (terrainID)
        {
			case "BEACH":
            case "HARBOUR":
            case "FOG":
            case "SEA":
                return 1;
			case "ROUGH_SEA":
            case "REAF":
				return 2;
        }
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_BOAT = new Constructor();
