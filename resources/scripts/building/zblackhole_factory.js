var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSprite("blackhole+factory", false);
            building.loadSpriteV2("blackhole+factory+mask", GameEnums.Recoloring_Matrix);
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

    this.actionList = ["ACTION_BLACKHOLEFACTORY_DOOR1", "ACTION_BLACKHOLEFACTORY_DOOR2", "ACTION_BLACKHOLEFACTORY_DOOR3"];

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
    this.canBuildingBePlaced = function(terrain, building)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZBLACKHOLE_FACTORY.getBuildingWidth(), ZBLACKHOLE_FACTORY.getBuildingHeigth());
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
var ZBLACKHOLE_FACTORY = new Constructor();
