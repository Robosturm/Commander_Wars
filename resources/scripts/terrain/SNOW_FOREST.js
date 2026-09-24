var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.getDefaultPalette = function()
    {
        return "palette_snow+aw1";
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEFOREST.loadBase(terrain, "SNOW_FOREST", "snow_forest+style1", map)
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        __BASEFOREST.loadOverlay(terrain, "SNOW_FOREST", "snow_forest+style1", map);
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("snow_forest+style1")
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return __BASEFOREST.getBaseOverlayTerrainSprites("snow_forest+style1");
    };
};
Constructor.prototype = __BASEFOREST;
var SNOW_FOREST = new Constructor();
