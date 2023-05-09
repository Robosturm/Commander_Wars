var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 3;
    };
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("omega+base+mask", GameEnums.Recoloring_Matrix);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("omega+base", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("omega+base+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
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
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZOMEGA_BASE.getBuildingWidth(), ZOMEGA_BASE.getBuildingHeigth(), map);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_pipeline";
    };
    this.getDescription = function()
    {
        return qsTr("Black Hole Omega Base meant to launch a giant Rocket on the Earth to destroy it.");
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return "back_missile+0";
    };
}

Constructor.prototype = BUILDING;
var ZOMEGA_BASE = new Constructor();
