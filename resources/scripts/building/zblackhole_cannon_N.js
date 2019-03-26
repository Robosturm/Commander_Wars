var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(100);
    };
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        building.loadSprite("black_cannon+N", false);
        building.loadSprite("black_cannon+N+mask", true);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_BLACKHOLECANNON_FIRE";
    };
    this.startOfTurn = function(building)
    {
        building.setFireCount(1);
    };
    this.getActionTargetFields = function(building)
    {
        return globals.getShotFields(1, 10, 0, -1);
    };
    this.getActionTargetOffset = function(building)
    {
        // offset for large buildings since there reference point is bound to the lower right corner.
        return Qt.point(-1, -2);
    };
    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 3;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 3;
    };
    this.canBuildingBePlaced = function(terrain)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, ZBLACKHOLE_CANNON_N.getBuildingWidth(), ZBLACKHOLE_CANNON_N.getBuildingHeigth());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
}

Constructor.prototype = BUILDING;
var ZBLACKHOLE_CANNON_N = new Constructor();
