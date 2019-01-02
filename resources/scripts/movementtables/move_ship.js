var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Ship");
    };
    this.getMovementpoints = function(terrainID)
    {
        switch (terrainID)
        {
            case "HARBOUR":
            case "SEA":
                return 1;
			case "ROUGH_SEA":
            case "REAF":
				return 2;
        }
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_SHIP = new Constructor();
