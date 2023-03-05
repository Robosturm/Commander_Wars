var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.getDefaultPalette = function()
    {
        return "palette_snow";
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEWASTELAND.loadBase(terrain, "snow_wasteland", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEWASTELAND.getSprites("snow_wasteland");
    };
};
Constructor.prototype = __BASEWASTELAND;
var SNOW_WASTELAND = new Constructor();
