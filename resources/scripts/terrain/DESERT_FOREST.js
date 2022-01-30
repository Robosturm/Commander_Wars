var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain)
    {
        __BASEFOREST.loadBase(terrain, "DESERT_FOREST", "desert_forest+style1")
    };
    this.loadOverlaySprite = function(terrain)
    {
        __BASEFOREST.loadOverlay(terrain, "DESERT_FOREST", "desert_forest+style1");
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("desert_forest+style1")
    };
};
Constructor.prototype = __BASEFOREST;
var DESERT_FOREST = new Constructor();
