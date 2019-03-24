var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(100);
    };
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        building.loadSprite("crystall", false);
		building.loadSprite("crystall+mask", true);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_CRYSTALL_HEAL";
    };
    this.startOfTurn = function(building)
    {
        building.setFireCount(1);
    };
}

Constructor.prototype = BUILDING;
var CRYSTALL = new Constructor();
