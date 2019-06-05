var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("temporary_harbour", false);
            building.loadSprite("temporary_harbour+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("temporary_harbour+neutral", false);
        }
    };
    this.addCaptureAnimationBuilding = function(animation, building, startColor, capturedColor)
    {
        animation.addBuildingSprite("harbour+mask", startColor , capturedColor, true);
        animation.addBuildingSprite("harbour", startColor , capturedColor, false);
    };
    this.getName = function()
    {
        return qsTr("Temporary Harbour");
    };

    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "";
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getConstructionList = function(building)
    {
        return ["CANNONBOAT", "BLACK_BOAT", "LANDER", "CRUISER", "SUBMARINE", "DESTROYER", "BATTLESHIP", "AIRCRAFTCARRIER"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_harbour";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_harbour";
    };
    this.getDefense = function()
    {
        return 1;
    };
}

Constructor.prototype = BUILDING;
var TEMPORARY_HARBOUR = new Constructor();
