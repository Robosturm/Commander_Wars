var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.baseTerrainId = "WASTE";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEWELD.loadBase(terrain, "waste_weld", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEWELD.getSprites("waste_weld")
    };
    this.onDestroyed = function(terrain, map)
    {
        __BASEWELD.onBaseDestroyed(terrain, "WASTE_DESTROYEDWELD", map);
    };
};
Constructor.prototype = __BASEWELD;
var WASTE_WELD = new Constructor();
