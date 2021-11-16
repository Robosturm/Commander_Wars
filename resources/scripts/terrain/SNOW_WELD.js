var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain)
    {
        __BASEWELD.loadBase(terrain, "snow_weld")
    };
    this.getTerrainSprites = function()
    {
        return __BASEWELD.getSprites("snow_weld")
    };
    this.onDestroyed = function(terrain)
    {
        __BASEWELD.onBaseDestroyed(terrain, "SNOW_DESTROYEDWELD");
    };
};
Constructor.prototype = __BASEWELD;
var SNOW_WELD = new Constructor();
