var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain)
    {
        __BASEWELD.loadBase(terrain, "desert_weld")
    };
    this.getTerrainSprites = function()
    {
        return __BASEWELD.getSprites("desert_weld")
    };
    this.onDestroyed = function(terrain)
    {
        __BASEWELD.onBaseDestroyed(terrain, "DESERT_DESTROYEDWELD");
    };
};
Constructor.prototype = __BASEWELD;
var DESERT_WELD = new Constructor();
