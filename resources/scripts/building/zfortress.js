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
            building.loadSprite("fortress", false);
            building.loadSpriteV2("fortress+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            building.loadSprite("fortress+neutral", false);
        }
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Fortress");
    };
    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 4;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 4;
    };
    this.canBuildingBePlaced = function(terrain, building)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZFORTRESS.getBuildingWidth(), ZFORTRESS.getBuildingHeigth());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getDescription = function()
    {
        return qsTr("Black Hole Fortress that defends a position has no other effects.");
    };
}

Constructor.prototype = BUILDING;
var ZFORTRESS = new Constructor();
