var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSprite("omega+base", false);
            building.loadSpriteV2("omega+base+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            building.loadSprite("omega+base+neutral", false);
        }
    };
    this.getDefense = function(building)
    {
        return 0;
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Omega Base");
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
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZOMEGA_BASE.getBuildingWidth(), ZOMEGA_BASE.getBuildingHeigth());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_pipeline";
    };
    this.getDescription = function()
    {
        return qsTr("Black Hole Omega Base meant to launch a giant Rocket on the Earth to destroy it.");
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_missile+0";
    };
}

Constructor.prototype = BUILDING;
var ZOMEGA_BASE = new Constructor();
