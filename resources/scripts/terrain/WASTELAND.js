var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain)
    {
        __BASEWASTELAND.loadBase(terrain, "wasteland")
    };
    this.getTerrainSprites = function()
    {
        return __BASEWASTELAND.getSprites("wasteland");
    };
};
Constructor.prototype = __BASEWASTELAND;
var WASTELAND = new Constructor();
