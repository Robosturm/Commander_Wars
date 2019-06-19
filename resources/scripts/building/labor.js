var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("labor", false);
            building.loadSprite("labor+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("labor+neutral", false);
        }
    };
    this.getName = function()
    {
        return qsTr("Labor");
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_BUILD_UNITS";
    };

    this.getConstructionList = function(building)
    {
        return ["HOELLIUM"];
    };
}

Constructor.prototype = BUILDING;
var LABOR = new Constructor();
