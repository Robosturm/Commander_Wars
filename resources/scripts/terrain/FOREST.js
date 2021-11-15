var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain)
    {
        __BASEFOREST.loadBase(terrain, "FOREST", "forest_style2")
    };
    this.loadOverlaySprite = function(terrain)
    {
        __BASEFOREST.loadOverlay(terrain, "FOREST", "forest_style2");
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("forest_style2")
    };
};
Constructor.prototype = __BASEFOREST;
var FOREST = new Constructor();
