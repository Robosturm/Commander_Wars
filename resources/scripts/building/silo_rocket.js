var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 6;
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("silo+rocket", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        building.loadSpriteV2("silo+rocket+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_silo";
    };

    this.getName = function()
    {
        return qsTr("Silo");
    };

    this.getDescription = function()
    {
        return qsTr("<r>A single missile can be launched from here, by an Infantry unit. The missile's blast damages units across a </r><div c='#00ff00'>13 square </div><r>area, including your own.</r>");
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return "back_missile+0";
    };
    this.getVision = function(building)
    {
        return -1;
    };
}

Constructor.prototype = BUILDING;
var SILO_ROCKET = new Constructor();
