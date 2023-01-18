var Constructor = function()
{    
    this.init = function (building)
    {
        building.setAlwaysVisble(true);
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("nest_factory", false);
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
        return qsTr("Nest factory door, which is able to produce a none naval unit each turn.");
    };
    this.getBuildingWidth = function()
    {
        return 1;
    };
    this.getBuildingHeigth = function()
    {
        return 4;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZNEST_FACTORY.getBuildingWidth(), ZNEST_FACTORY.getBuildingHeigth(), map);
    };
    this.actionList = ["ACTION_NEST_FACTORY_DOOR"];
    this.startOfTurn = function(building, map)
    {
        building.setFireCount(1);
    };
    this.getConstructionList = function(building)
    {
        var unitIds = [];
        var map = building.getMap();
        var units = map.getAllUnitIDs();
        var buildlist = building.getOwner().getBuildList();
        for (var i = 0; i < units.length; i++)
        {
            // check all units if they can move over this terrain
            if (buildlist.includes(units[i]) &&
                Global[units[i]].getUnitType() !== GameEnums.UnitType_Naval &&
                Global[units[i]].getCOSpecificUnit() === false)
            {
                unitIds.push(units[i]);
            }
        }
        return unitIds;
    }
}

Constructor.prototype = BUILDING;
var ZNEST_FACTORY = new Constructor();
