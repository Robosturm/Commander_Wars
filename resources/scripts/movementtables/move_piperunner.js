var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Piperunner");
    };
    this.getMovementpoints = function(terrainID)
    {
        switch (terrainID)
        {
			case "WELD":
            case "PIPELINE":
				return 1;
        }
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_PIPERUNNER = new Constructor();
