var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.getDefaultPalette = function()
    {
        return "palette_clear";
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASERUIN.loadBase(terrain, "ruin", map);
    };
    this.getTerrainSprites = function()
    {
        return __BASERUIN.getSprites("ruin")
    };
};
Constructor.prototype = __BASERUIN;
var RUIN = new Constructor();
