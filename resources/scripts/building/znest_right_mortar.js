var Constructor = function()
{    
    this.init = function (building)
    {
        building.setAlwaysVisble(true);
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("nest_mortar_right", false);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function()
    {
        return 0;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getName = function()
    {
        return qsTr("Nest");
    };
    this.getDescription = function()
    {
        return qsTr("Nest of Dark Matter.");
    };
    this.getBuildingWidth = function()
    {
        return 6;
    };
    this.getBuildingHeigth = function()
    {
        return 6;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZNEST_RIGHT_MORTAR.getBuildingWidth(), ZNEST_RIGHT_MORTAR.getBuildingHeigth(), map);
    };
}

Constructor.prototype = BUILDING;
var ZNEST_RIGHT_MORTAR = new Constructor();
