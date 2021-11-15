var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.loadBaseSprite = function(terrain)
    {
        __BASEFOREST.loadBase(terrain, "SNOW_FOREST", "snow_forest+style1")
    };
    this.loadOverlaySprite = function(terrain)
    {
        __BASEFOREST.loadOverlay(terrain, "SNOW_FOREST", "snow_forest+style1");
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("snow_forest+style1")
    };

    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
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
            terrain.loadBaseTerrain("SNOW");
        }
    };
};
Constructor.prototype = __BASEFOREST;
var SNOW_FOREST = new Constructor();
