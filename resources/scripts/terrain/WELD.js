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
        __BASEWELD.loadBase(terrain, "weld", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEWELD.getSprites("weld")
    };
    this.onDestroyed = function(terrain, map)
    {
        __BASEWELD.onBaseDestroyed(terrain, "DESTROYEDWELD", map);
    };
};
Constructor.prototype = __BASEWELD;
var WELD = new Constructor();
