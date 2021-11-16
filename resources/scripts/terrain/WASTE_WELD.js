var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.baseTerrainId = "WASTE";
    this.loadBaseSprite = function(terrain)
    {
        __BASEWELD.loadBase(terrain, "waste_weld")
    };
    this.getTerrainSprites = function()
    {
        return __BASEWELD.getSprites("waste_weld")
    };
    this.onDestroyed = function(terrain)
    {
        __BASEWELD.onBaseDestroyed(terrain, "WASTE_DESTROYEDWELD");
    };
};
Constructor.prototype = __BASEWELD;
var WASTE_WELD = new Constructor();
