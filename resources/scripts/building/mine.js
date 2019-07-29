var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("mine", false);
            building.loadSprite("mine+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("mine+neutral", false);
        }
    };
    this.getBaseIncome = function()
    {
        return 3000;
    };
    this.getName = function()
    {
        return qsTr("Mine");
    };

    this.getDescription = function()
    {
        return qsTr("Ore mine. Once captured produces a lot of funds, but can't repair units.");
    };
}

Constructor.prototype = BUILDING;
var MINE = new Constructor();
