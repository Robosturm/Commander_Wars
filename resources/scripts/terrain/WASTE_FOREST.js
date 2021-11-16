var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.baseTerrainId = "WASTE";
    this.loadBaseSprite = function(terrain)
    {
        __BASEFOREST.loadBase(terrain, "WASTE_FOREST", "waste_forest+style0")
    };
    this.loadOverlaySprite = function(terrain)
    {
        __BASEFOREST.loadOverlay(terrain, "WASTE_FOREST", "waste_forest+style0");
    };

    this.getTerrainSprites = function()
    {
        return __BASEFOREST.getSprites("waste_forest+style0")
    };
};
Constructor.prototype = __BASEFOREST;
var WASTE_FOREST = new Constructor();
