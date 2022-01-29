var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.baseTerrainId = "WASTE";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEWASTELAND.loadBase(terrain, "waste_wasteland", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEWASTELAND.getSprites("waste_wasteland");
    };
};
Constructor.prototype = __BASEWASTELAND;
var WASTE_WASTELAND = new Constructor();
