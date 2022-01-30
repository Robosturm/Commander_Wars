var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain)
    {
        __BASEWASTELAND.loadBase(terrain, "snow_wasteland")
    };
    this.getTerrainSprites = function()
    {
        return __BASEWASTELAND.getSprites("snow_wasteland");
    };
};
Constructor.prototype = __BASEWASTELAND;
var SNOW_WASTELAND = new Constructor();
