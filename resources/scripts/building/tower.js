var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("tower", false);
            building.loadSprite("tower+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("tower+neutral", false);
        }
    };
}

Constructor.prototype = BUILDING;
var TOWER = new Constructor();
