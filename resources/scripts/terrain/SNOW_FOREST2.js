var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.getDefaultPalette = function()
    {
        return "palette_snow+awds";
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEFOREST.loadBase(terrain, "SNOW_FOREST2", "snow_forest+style2", map)
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        __BASEFOREST.loadOverlay(terrain, "SNOW_FOREST2", "snow_forest+style2", map);
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("snow_forest+style2")
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return __BASEFOREST.getBaseOverlayTerrainSprites("snow_forest+style2");
    };
    this.getShowInWiki = function()
    {
        return false;
    };
};
Constructor.prototype = __BASEFOREST;
var SNOW_FOREST2 = new Constructor();
