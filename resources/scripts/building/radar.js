var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("radar", false);
            building.loadSpriteV2("radar+mask", GameEnums.Recoloring_Table);
        }
        else
        {
            // neutral player
            building.loadSprite("radar+neutral", false);
        }
    };
    this.getBaseIncome = function()
    {
        return 0;
    };

    this.getVisionBonus = function()
    {
        return 0;
    };

    this.getName = function()
    {
        return qsTr("Radar");
    };

    this.getDescription = function()
    {
        return "<r>" + qsTr("In Fog of War, Radars make a ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("5 square ") + "</div>" +
               "<r>" + qsTr("area around them visible.") + "</r>";
    };

    this.getVision = function(building)
    {
        return 5;
    };

    this.getVisionHide = function(building)
    {
        return true;
    };
}

Constructor.prototype = BUILDING;
var RADAR = new Constructor();
