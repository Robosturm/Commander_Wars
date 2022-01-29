var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEFOREST.loadBase(terrain, "FOREST2", "forest_style0", map)
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        __BASEFOREST.loadOverlay(terrain, "FOREST2", "forest_style0", map);
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("forest_style0")
    };
};
Constructor.prototype = __BASEFOREST;
var FOREST2 = new Constructor();
