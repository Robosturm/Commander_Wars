var Constructor = function()
{
    
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("harbour", false);
            building.loadSpriteV2("harbour+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            // neutral player
            building.loadSprite("harbour+neutral", false);
        }
    };
    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("harbour+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Matrix);
    };
    this.getName = function()
    {
        return qsTr("Harbour");
    };

    this.actionList = ["ACTION_BUILD_UNITS"];
    this.constructionList = ["CANNONBOAT", "BLACK_BOAT", "LANDER", "CRUISER", "SUBMARINE", "DESTROYER", "BATTLESHIP", "AIRCRAFTCARRIER"];
    this.getConstructionList = function(building)
    {
        return HARBOUR.constructionList;
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "back_" + weatherModifier + "harbour";
    };

    this.getDescription = function()
    {
        return qsTr("<r>Once captured can be used for </r><div c='#00ff00'>production and resupplying </div><r>of </r><div c='#00ff00'>naval </div><r>units.</r>");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getRepairTypes = function(building)
    {
        return [GameEnums.UnitType_Hovercraft, GameEnums.UnitType_Naval];
    };
}

Constructor.prototype = BUILDING;
var HARBOUR = new Constructor();
