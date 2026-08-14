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
        building.loadSpriteV2("grand_bolt+eye+middle+destroyed+mask", GameEnums.Recoloring_Matrix);
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
        return 7;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 1;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZGRAND_BOLT_EYE_MIDDLE_DESTROYED.getBuildingWidth(), ZGRAND_BOLT_EYE_MIDDLE_DESTROYED.getBuildingHeigth(), map);
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
var ZGRAND_BOLT_EYE_MIDDLE_DESTROYED= new Constructor();
