var Constructor = function()
{
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("blackhole+factory+mask", GameEnums.Recoloring_Matrix);
        building.loadSpriteV2("blackhole+factory+desert_pipe", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("blackhole+factory", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("blackhole+factory+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
    this.getName = function()
    {
        return qsTr("Black Hole desert factory");
    };
    this.getShowInEditor = function()
    {
        return false;
    };
};

Constructor.prototype = __ZBLACKHOLE_FACTORYBASE;
var ZBLACKHOLE_FACTORYDESERT = new Constructor();
