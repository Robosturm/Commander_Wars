var Constructor = function()
{    
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("nest_center_laser_right+destroyed", false, 400, Qt.point(0, building.getImageSize()));
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
var ZNEST_CENTER_LASER_RIGHT_DESTROYED = new Constructor();
