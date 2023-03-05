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
        __BASERUIN.loadBase(terrain, "snow_ruin", map);
    };
    this.getTerrainSprites = function()
    {
        return __BASERUIN.getSprites("snow_ruin");
    };
};
Constructor.prototype = __BASERUIN;
var SNOW_RUIN = new Constructor();
