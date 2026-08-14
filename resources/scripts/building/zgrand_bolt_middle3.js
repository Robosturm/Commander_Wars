var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 4;
    };
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };

    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("grand_bolt+middle3+mask", GameEnums.Recoloring_Matrix);
    };
    this.getDefense = function(building)
    {
        return 0;
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Grand Bolt");
    };
    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 15;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 1;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZGRAND_BOLT_MIDDLE3.getBuildingWidth(), ZGRAND_BOLT_MIDDLE3.getBuildingHeigth(), map);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getDescription = function()
    {
        return qsTr("The grand bolt producing more ooziums.");
    };
}

Constructor.prototype = BUILDING;
var ZGRAND_BOLT_MIDDLE3 = new Constructor();
