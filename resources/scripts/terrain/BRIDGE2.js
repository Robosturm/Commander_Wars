var Constructor = function()
{
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        __BASEBRIDGE.loadSprite(terrain, "bridge_river", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge_river")
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge_river")
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return __BASEBRIDGE.getTerrainAnimationBackgroundRand(unit, terrain, defender, map, 1);
    };
};
Constructor.prototype = __BASEBRIDGE;
var BRIDGE2 = new Constructor();
