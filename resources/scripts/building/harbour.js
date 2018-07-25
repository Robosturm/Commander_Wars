var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("harbour", false);
            building.loadSprite("harbour+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("harbour+neutral", false);
        }
    };
}

Constructor.prototype = BUILDING;
var HARBOUR = new Constructor();
