var Constructor = function()
{    
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("nest_laser+S", false, 400, Qt.point(0, building.getImageSize()));
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
        return qsTr("Nest of Dark Matter. Currently only a visual.");
    };
    this.getBuildingWidth = function()
    {
        return 3;
    };
    this.getBuildingHeigth = function()
    {
        return 1;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZNEST_NEST_LASER.getBuildingWidth(), ZNEST_NEST_LASER.getBuildingHeigth(), map);
    };
    this.startOfTurn = function(building, map)
    {
        building.unloadSprites();
        var variables = building.getVariables();
        var day = map.getCurrentDay();
        if (day % 3 === 1)
        {
            building.loadSprite("nest_laser+S", false, 400, Qt.point(0, building.getImageSize()));
        }
        else if (day % 3 === 2)
        {
            building.loadSprite("nest_laser+SE", false, 400, Qt.point(0, building.getImageSize()));
        }
        else if (day % 3 === 0)
        {
            building.loadSprite("nest_laser+SW", false, 400, Qt.point(0, building.getImageSize()));
        }
    };
    this.usesMapLayer = function()
    {
        return true;
    };
}

Constructor.prototype = BUILDING;
var ZNEST_NEST_LASER = new Constructor();
