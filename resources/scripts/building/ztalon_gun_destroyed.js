var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 5;
    };
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building, neutral, map)
    {
         building.loadSpriteV2("talon+gun+destroyed", GameEnums.Recoloring_Matrix);
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
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZTALON_GUN_DESTROYED.getBuildingWidth(), ZTALON_GUN_DESTROYED.getBuildingHeigth(), map);
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
        return qsTr("Destroyed Talon Gun of Dark Matter.");
    };
    this.getVision = function(building)
    {
        return -1;
    };
}

Constructor.prototype = BUILDING;
var ZTALON_GUN_DESTROYED = new Constructor();
