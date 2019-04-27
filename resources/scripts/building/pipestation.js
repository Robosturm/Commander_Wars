var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("pipestation", false);
            building.loadSprite("pipestation+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("pipestation+neutral", false);
        }
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_BUILD_UNITS";
    };

    this.getConstructionList = function(building)
    {
        return ["PIPERUNNER"];
    };
    this.getBaseIncome = function()
    {
        return 2000;
    };
    this.getName = function()
    {
        return qsTr("Pipestation");
    };
}

Constructor.prototype = BUILDING;
var PIPESTATION = new Constructor();
