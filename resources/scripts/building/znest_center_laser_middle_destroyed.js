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
        building.loadSpriteV2("nest_center_laser_middle+destroyed", GameEnums.Recoloring_Matrix, 400, Qt.point(0, building.getImageSize()));
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
        return qsTr("Destroyed nest laser.");
    };
    this.usesMapLayer = function()
    {
        return true;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return map.onMap(terrain.getX(), terrain.getY() + 1);
    };
}

Constructor.prototype = BUILDING;
var ZNEST_CENTER_LASER_MIDDLE_DESTROYED = new Constructor();
