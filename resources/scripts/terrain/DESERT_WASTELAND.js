var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.getDefaultPalette = function()
    {
        return "palette_desert";
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEWASTELAND.loadBase(terrain, "desert_wasteland", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEWASTELAND.getSprites("desert_wasteland");
    };
};
Constructor.prototype = __BASEWASTELAND;
var DESERT_WASTELAND = new Constructor();
