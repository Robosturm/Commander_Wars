var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEFOREST.loadBase(terrain, "FOREST1", "forest_style1", map)
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        __BASEFOREST.loadOverlay(terrain, "FOREST1", "forest_style1", map);
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("forest_style1")
    };
};
Constructor.prototype = __BASEFOREST;
var FOREST1 = new Constructor();
