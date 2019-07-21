var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Piperunner");
    };
    this.getMovementpoints = function(terrain)
    {
        switch (terrain.getID())
        {
			case "WELD":
            case "FACTORY":
            case "PIPELINE":
            case "DESERT_WELD":
            case "DESERT_PIPELINE":
				return 1;
            case "SNOW_WELD":
            case "SNOW_PIPELINE":
                return 2;
        }
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_PIPERUNNER = new Constructor();
