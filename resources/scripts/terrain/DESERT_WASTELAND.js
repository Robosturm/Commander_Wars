var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain)
    {
        __BASEWASTELAND.loadBase(terrain, "desert_wasteland")
    };
    this.getTerrainSprites = function()
    {
        return __BASEWASTELAND.getSprites("desert_wasteland");
    };
};
Constructor.prototype = __BASEWASTELAND;
var DESERT_WASTELAND = new Constructor();
