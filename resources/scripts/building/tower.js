var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("tower", false);
            building.loadSprite("tower+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("tower+neutral", false);
        }
    };
    this.getBaseIncome = function()
    {
        return 0;
    };

    this.getOffensiveBonus = function(building)
    {
        return 5;
    };

    this.getDefensiveBonus = function(building)
    {
        return 5;
    };

    this.getName = function()
    {
        return qsTr("Tower");
    };
}

Constructor.prototype = BUILDING;
var TOWER = new Constructor();
