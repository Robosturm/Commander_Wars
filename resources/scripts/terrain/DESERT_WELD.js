var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEWELD.loadBase(terrain, "desert_weld", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEWELD.getSprites("desert_weld")
    };
    this.onDestroyed = function(terrain, map)
    {
        __BASEWELD.onBaseDestroyed(terrain, "DESERT_DESTROYEDWELD", map);
    };
};
Constructor.prototype = __BASEWELD;
var DESERT_WELD = new Constructor();
