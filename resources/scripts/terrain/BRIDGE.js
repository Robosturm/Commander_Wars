var Constructor = function()
{
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        __BASEBRIDGE.loadSprite(terrain, "bridge+style1", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge+style1")
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge+style1")
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return __BASEBRIDGE.getTerrainAnimationBackgroundRand(unit, terrain, defender, map, 0);
    };

    this.canBePlaced = function(x, y, map)
    {
        return __BASEBRIDGE.canBePlacedBaseId(x, y, map, "BRIDGE");
    };
};
Constructor.prototype = __BASEBRIDGE;
var BRIDGE = new Constructor();
