var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.getDefaultPalette = function()
    {
        return "palette_snow";
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASESTREET.loadBase(terrain, "snow_street+style0", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("snow_street+style0")
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        __BASESTREET.loadBaseOverlaySprite("snow_street+style0", terrain, map);
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return __BASESTREET.getBaseOverlayTerrainSprites("snow_street+style0");
    };
};
Constructor.prototype = __BASESTREET;
var SNOW_STREET = new Constructor();
