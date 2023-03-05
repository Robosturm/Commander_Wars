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
        __BASEDESTROYED_WELD.loadBase(terrain, "destroyedweld", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEDESTROYED_WELD.getSprites("destroyedweld");
    };
};
Constructor.prototype = __BASEDESTROYED_WELD;
var DESTROYEDWELD = new Constructor();
