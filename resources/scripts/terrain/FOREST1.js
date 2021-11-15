var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain)
    {
        __BASEFOREST.loadBase(terrain, "FOREST1", "forest_style1")
    };
    this.loadOverlaySprite = function(terrain)
    {
        __BASEFOREST.loadOverlay(terrain, "FOREST1", "forest_style1");
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("forest_style1")
    };
};
Constructor.prototype = __BASEFOREST;
var FOREST1 = new Constructor();
