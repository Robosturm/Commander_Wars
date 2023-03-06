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
        __BASEMOUNTAIN.loadBase(terrain, "desert_rock", map)
    };

    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("desert_rock");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var DESERT_ROCK = new Constructor();
