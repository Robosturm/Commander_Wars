var Constructor = function()
{
    this.loadBaseSprite = function(terrain, currentTerrainID)
    {
        __BASEBRIDGE.loadSprite(terrain, "bridge+style1")
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge+style1")
    };
};
Constructor.prototype = __BASEBRIDGE;
var BRIDGE = new Constructor();
