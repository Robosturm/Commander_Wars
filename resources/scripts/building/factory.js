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

    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_BUILD_UNITS";
    };

    this.getConstructionList = function()
    {
        return ["INFANTRY", "MECH", "MOTORBIKE", "RECON", "APC", "FLARE",
                "ARTILLERY", "LIGHT_TANK", "FLAK", "ANTITANKCANNON",
                "HEAVY_TANK", "MISSILE", "ROCKETTHROWER", "MEGATANK"];
    };
}

Constructor.prototype = BUILDING;
var FACTORY = new Constructor();
