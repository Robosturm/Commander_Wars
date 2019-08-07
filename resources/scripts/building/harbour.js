var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
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
    this.addCaptureAnimationBuilding = function(animation, building, startColor, capturedColor)
    {
        animation.addBuildingSprite("harbour+mask", startColor , capturedColor, true);
        animation.addBuildingSprite("harbour", startColor , capturedColor, false);
    };
    this.getName = function()
    {
        return qsTr("Harbour");
    };

    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_BUILD_UNITS";
    };
    this.getConstructionList = function(building)
    {
        return ["HOVERCRAFT", "ARTILLERYCRAFT", "HOVERFLAK", "HEAVY_HOVERCRAFT",
                "CANNONBOAT", "BLACK_BOAT", "LANDER", "CRUISER", "SUBMARINE", "DESTROYER", "BATTLESHIP", "AIRCRAFTCARRIER"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_harbour";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_harbour";
    };

    this.getDescription = function()
    {
        return qsTr("Once captured can be used for <div c='00ff00'>production and resupplying </div>of <div c='00ff00'>naval </div>units.");
    };
}

Constructor.prototype = BUILDING;
var HARBOUR = new Constructor();
