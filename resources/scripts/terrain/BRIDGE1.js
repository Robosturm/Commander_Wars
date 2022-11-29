var Constructor = function()
{
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        __BASEBRIDGE.loadSprite(terrain, "bridge+style0", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge+style0")
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge+style0")
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return __BASEBRIDGE.getTerrainAnimationBackgroundRand(unit, terrain, defender, map, 0);
    };
    this.canBePlaced = function(x, y, map)
    {
        return __BASEBRIDGE.canBePlacedBaseId(x, y, map, "BRIDGE1");
    };
    this.getName = function()
    {
        return qsTr("Bridge");
    };
};
Constructor.prototype = __BASEBRIDGE;
var BRIDGE1 = new Constructor();
