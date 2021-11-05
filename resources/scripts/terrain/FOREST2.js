var Constructor = function()
{
    this.loadBaseSprite = function(terrain)
    {
        __BASEFOREST.loadBase(terrain, "FOREST2", "forest_style0")
    };
    this.loadOverlaySprite = function(terrain)
    {
        __BASEFOREST.loadOverlay(terrain, "FOREST2", "forest_style0");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return __BASEFOREST.getSprites("forest_style0")
    };
};
Constructor.prototype = __BASEFOREST;
var FOREST2 = new Constructor();
