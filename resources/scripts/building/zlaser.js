var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(100);
    };
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        building.loadSprite("laser", false);
		building.loadSprite("laser+mask", true);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_LASER_FIRE";
    };
    this.startOfTurn = function(building)
    {
        building.setFireCount(1);
    };
    this.getActionTargetFields = function(building)
    {
        var targets = globals.getEmptyPointArray();
        // laser to not fire infinitly but the range is still fucking huge :)
        for (var i = 1; i < 20; i++)
        {
            targets.append(Qt.point(0, i));
            targets.append(Qt.point(0, -i));
            targets.append(Qt.point(i, 0));
            targets.append(Qt.point(-i, 0));
        }
        return targets;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
}

Constructor.prototype = BUILDING;
var ZLASER = new Constructor();
