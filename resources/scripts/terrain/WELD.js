var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain)
    {
        __BASEWELD.loadBase(terrain, "weld")
    };
    this.getTerrainSprites = function()
    {
        return __BASEWELD.getSprites("weld")
    };
    this.onDestroyed = function(terrain)
    {
        __BASEWELD.onBaseDestroyed(terrain, "DESTROYEDWELD");
    };
};
Constructor.prototype = __BASEWELD;
var WELD = new Constructor();
