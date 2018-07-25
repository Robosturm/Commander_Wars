var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("hq+os", false);
            building.loadSprite("hq+os+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("hq+neutral", false);
        }
    };
}

Constructor.prototype = BUILDING;
var HQ = new Constructor();
