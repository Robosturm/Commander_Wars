var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.baseTerrainId = "SNOW";
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
};
Constructor.prototype = __BASEFOREST;
var SNOW_FOREST = new Constructor();
