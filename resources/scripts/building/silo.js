var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 6;
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("silo", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        building.loadSpriteV2("silo+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
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
        return qsTr("Empty missile silo which provides some defence.");
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return "back_missile+1";
    };
    this.getVision = function(building)
    {
        return -1;
    };
}

Constructor.prototype = BUILDING;
var SILO = new Constructor();
