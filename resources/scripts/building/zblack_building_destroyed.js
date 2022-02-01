var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building)
    {
		building.loadSprite("black_building_destroyed", false);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function(building)
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
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZBLACK_BUILDING_DESTROYED.getBuildingWidth(), ZBLACK_BUILDING_DESTROYED.getBuildingHeigth(), map);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };

    this.getName = function()
    {
        return qsTr("Ruins");
    };

    this.getDescription = function()
    {
        return qsTr("Destroyed Black Hole Building.");
    };
}

Constructor.prototype = BUILDING;
var ZBLACK_BUILDING_DESTROYED = new Constructor();
