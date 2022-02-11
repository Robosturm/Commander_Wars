var Constructor = function()
{    
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("nest_center_laser_right", false, 400, Qt.point(0, map.getImageSize()));
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
}

Constructor.prototype = BUILDING;
var ZNEST_CENTER_LASER_RIGHT = new Constructor();
