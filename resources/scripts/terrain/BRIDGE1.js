var Constructor = function()
{
    this.loadBaseSprite = function(terrain, currentTerrainID)
    {
        __BASEBRIDGE.loadSprite(terrain, "bridge+style0")
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge+style0")
    };
};
Constructor.prototype = __BASEBRIDGE;
var BRIDGE1 = new Constructor();
