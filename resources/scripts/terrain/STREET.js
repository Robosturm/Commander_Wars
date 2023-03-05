var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.getDefaultPalette = function()
    {
        return "palette_clear";
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASESTREET.loadBase(terrain, "street+style1", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("street+style1")
    };

    this.loadOverlaySprite = function(terrain, map)
    {
        __BASESTREET.loadBaseOverlaySprite("street+style1", terrain, map);
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return __BASESTREET.getBaseOverlayTerrainSprites("street+style1");
    };
};
Constructor.prototype = __BASESTREET;
var STREET = new Constructor();
