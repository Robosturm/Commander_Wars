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
				return 1;
        }
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_PIPERUNNER = new Constructor();
