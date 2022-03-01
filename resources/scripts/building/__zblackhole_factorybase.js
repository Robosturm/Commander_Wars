var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };

    this.getDefense = function(building)
    {
        return 0;
    };
    this.startOfTurn = function(building, map)
    {
        building.setFireCount(7);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Black Hole Factory");
    };

    this.actionList = ["ACTION_BLACKHOLEFACTORY_DOOR1", "ACTION_BLACKHOLEFACTORY_DOOR2", "ACTION_BLACKHOLEFACTORY_DOOR3"];

    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 3;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 4;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, __ZBLACKHOLE_FACTORYBASE.getBuildingWidth(), __ZBLACKHOLE_FACTORYBASE.getBuildingHeigth(), map);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };

    this.getDescription = function()
    {
        return qsTr("Black Hole Factory that can build 3 units each turn which can immediatly move.");
    };
}

Constructor.prototype = BUILDING;
var __ZBLACKHOLE_FACTORYBASE = new Constructor();
