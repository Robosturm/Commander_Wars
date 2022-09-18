var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEFOREST.loadBase(terrain, "DESERT_FOREST", "desert_forest+style1", map)
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        __BASEFOREST.loadOverlay(terrain, "DESERT_FOREST", "desert_forest+style1", map);
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("desert_forest+style1")
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return __BASEFOREST.getBaseOverlayTerrainSprites("desert_forest+style1");
    };
};
Constructor.prototype = __BASEFOREST;
var DESERT_FOREST = new Constructor();
