var Constructor = function()
{
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
        // array of sprites that can be selected as fix sprites for this terrain
        return __BASEFOREST.getSprites("forest_style3")
    };
};
Constructor.prototype = __BASEFOREST;
var FOREST3 = new Constructor();
