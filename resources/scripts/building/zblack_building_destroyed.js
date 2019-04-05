var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
    };
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
		building.loadSprite("black_building_destroyed", false);
    };
    this.getBaseIncome = function()
    {
        return 0;
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
        return BUILDING.canLargeBuildingPlaced(terrain, ZBLACK_BUILDING_DESTROYED.getBuildingWidth(), ZBLACK_BUILDING_DESTROYED.getBuildingHeigth());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };

    this.getName = function()
    {
        return qsTr("Ruins");
    };
}

Constructor.prototype = BUILDING;
var ZBLACK_BUILDING_DESTROYED = new Constructor();
