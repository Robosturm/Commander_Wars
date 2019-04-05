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
}

Constructor.prototype = BUILDING;
var LABOR = new Constructor();
