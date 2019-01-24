var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("airport", false);
            building.loadSprite("airport+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("airport+neutral", false);
        }
    };

    this.addCaptureAnimationBuilding = function(animation, building, startColor, capturedColor)
    {
        animation.addBuildingSprite("airport+mask", startColor , capturedColor, true);
        animation.addBuildingSprite("airport", startColor , capturedColor, false);
    };
}

Constructor.prototype = BUILDING;
var AIRPORT = new Constructor();
