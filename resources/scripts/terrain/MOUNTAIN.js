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
        __BASEMOUNTAIN.loadBase(terrain, "mountain", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("mountain");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var MOUNTAIN = new Constructor();
