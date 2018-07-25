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
}

Constructor.prototype = BUILDING;
var PIPESTATION = new Constructor();
