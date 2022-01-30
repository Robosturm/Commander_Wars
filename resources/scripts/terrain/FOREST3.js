var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain)
    {
        __BASEFOREST.loadBase(terrain, "FOREST3", "forest_style3")
    };
    this.loadOverlaySprite = function(terrain)
    {
        __BASEFOREST.loadOverlay(terrain, "FOREST3", "forest_style3");
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("forest_style3")
    };
};
Constructor.prototype = __BASEFOREST;
var FOREST3 = new Constructor();
