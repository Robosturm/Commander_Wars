var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
    };
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            building.loadSprite("blackhole+factory", false);
            building.loadSprite("blackhole+factory+mask", true);
        }
        else
        {
            building.loadSprite("blackhole+factory+neutral", false);
        }
    };
    this.startOfTurn = function(building)
    {
        building.setFireCount(7);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Black Hole Factory");
    };

    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_BLACKHOLEFACTORY_DOOR1,ACTION_BLACKHOLEFACTORY_DOOR2,ACTION_BLACKHOLEFACTORY_DOOR3";
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
    this.canBuildingBePlaced = function(terrain)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, ZBLACKHOLE_FACTORY.getBuildingWidth(), ZBLACKHOLE_FACTORY.getBuildingHeigth());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
}

Constructor.prototype = BUILDING;
var ZBLACKHOLE_FACTORY = new Constructor();
