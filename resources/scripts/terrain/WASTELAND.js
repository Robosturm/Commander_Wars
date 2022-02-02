var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEWASTELAND.loadBase(terrain, "wasteland", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEWASTELAND.getSprites("wasteland");
    };
};
Constructor.prototype = __BASEWASTELAND;
var WASTELAND = new Constructor();
