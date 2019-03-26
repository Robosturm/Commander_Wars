var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
    };
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        building.loadSprite("volcan", false);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 4;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 4;
    };
    this.canBuildingBePlaced = function(terrain)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, ZVOLCAN.getBuildingWidth(), ZVOLCAN.getBuildingHeigth());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
}

Constructor.prototype = BUILDING;
var ZVOLCAN = new Constructor();
