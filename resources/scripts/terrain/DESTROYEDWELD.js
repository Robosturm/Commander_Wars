var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain)
    {
        __BASEDESTROYED_WELD.loadBase(terrain, "destroyedweld")
    };
    this.getTerrainSprites = function()
    {
        return __BASEDESTROYED_WELD.getSprites("destroyedweld");
    };
};
Constructor.prototype = __BASEDESTROYED_WELD;
var DESTROYEDWELD = new Constructor();
