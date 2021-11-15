var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.loadBaseSprite = function(terrain)
    {
        __BASEFOREST.loadBase(terrain, "DESERT_FOREST1", "desert_forest+style0")
    };
    this.loadOverlaySprite = function(terrain)
    {
        __BASEFOREST.loadOverlay(terrain, "DESERT_FOREST1", "desert_forest+style0");
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("desert_forest+style0")
    };

    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("DESERT");
        }
    };
};
Constructor.prototype = __BASEFOREST;
var DESERT_FOREST1 = new Constructor();
