var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };

    this.getDefense = function(building)
    {
        return 0;
    };
    this.startOfTurn = function(building, map)
    {
        building.setFireCount(7);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };

    this.actionList = ["ACTION_BLACKHOLEFACTORY_DOOR1", "ACTION_BLACKHOLEFACTORY_DOOR2", "ACTION_BLACKHOLEFACTORY_DOOR3"];
    this.getConstructionList = function(building)
    {
        var xBuilding = building.getX();
        var y = building.getY() + 1;
        var map = building.getMap();
        var units = map.getAllUnitIDs();
        var buildlist = building.getOwner().getBuildList();
        var unitIds = [];
        var length = units.length;
        for (var x = xBuilding - 2; x <= xBuilding; ++x)
        {
            var terrain = map.getTerrain(x, y);
            for (var i = 0; i < length; i++)
            {
                if (unitIds.indexOf(units[i]) < 0)
                {
                    // check all units if they can move over this terrain
                    if (buildlist.includes(units[i]) &&
                        Global[Global[units[i]].getMovementType()].getMovementpoints(terrain, null, terrain, true, map) > 0 &&
                        Global[units[i]].getCOSpecificUnit() === false)
                    {
                        unitIds.push(units[i]);
                    }
                }
            }
        }
        return unitIds;
    };

    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 3;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 4;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, __ZBLACKHOLE_FACTORYBASE.getBuildingWidth(), __ZBLACKHOLE_FACTORYBASE.getBuildingHeigth(), map);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };

    this.getDescription = function()
    {
        return qsTr("Black Hole Factory that can build 3 units each turn which can immediatly move.");
    };
}

Constructor.prototype = BUILDING;
var __ZBLACKHOLE_FACTORYBASE = new Constructor();
