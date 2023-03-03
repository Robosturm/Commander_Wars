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
        building.loadSprite("nest_center_laser_left+destroyed", false, 400, Qt.point(0, building.getImageSize()));
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
}

Constructor.prototype = BUILDING;
var ZNEST_CENTER_LASER_LEFT_DESTROYED = new Constructor();
