var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("factory", false);
            building.loadSprite("factory+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("factory+neutral", false);
        }
    };
    this.addCaptureAnimationBuilding = function(animation, building, startColor, capturedColor)
    {
        animation.addBuildingSprite("factory+mask", startColor , capturedColor, true);
        animation.addBuildingSprite("factory", startColor , capturedColor, false);
    };
}

Constructor.prototype = BUILDING;
var FACTORY = new Constructor();
