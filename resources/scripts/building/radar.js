var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("radar", false);
            building.loadSprite("radar+mask", true);
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
        return qsTr("In Fog of War, Radars make a <div c='00ff00'>5 square </div>area around them visible.");
    };

    this.getVision = function()
    {
        return 5;
    };
}

Constructor.prototype = BUILDING;
var RADAR = new Constructor();
