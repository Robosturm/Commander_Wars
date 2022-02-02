var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEFOREST.loadBase(terrain, "DESERT_FOREST1", "desert_forest+style0", map)
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        __BASEFOREST.loadOverlay(terrain, "DESERT_FOREST1", "desert_forest+style0", map);
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("desert_forest+style0")
    };
};
Constructor.prototype = __BASEFOREST;
var DESERT_FOREST1 = new Constructor();
