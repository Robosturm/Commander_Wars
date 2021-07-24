var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("labor", false);
            building.loadSpriteV2("labor+mask", GameEnums.Recoloring_Table);
        }
        else
        {
            // neutral player
            building.loadSprite("labor+neutral", false);
        }
    };
    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("labor+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Table);
        animation.addBuildingSprite("labor", startPlayer , capturedPlayer, GameEnums.Recoloring_None);
    };
    this.getName = function()
    {
        return qsTr("Labor");
    };
    this.actionList = ["ACTION_BUILD_UNITS"];
    this.constructionList = ["HOELLIUM"];
    this.getConstructionList = function(building)
    {
        return LABOR.constructionList;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Once captured can be used for </r><div c='#00ff00'>production</div><r> of </r><div c='#00ff00'>oozium</div><r>units.</r>");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };
}

Constructor.prototype = BUILDING;
var LABOR = new Constructor();
