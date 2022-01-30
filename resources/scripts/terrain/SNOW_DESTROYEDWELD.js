var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain)
    {
        __BASEDESTROYED_WELD.loadBase(terrain, "snow_destroyedweld")
    };
    this.getTerrainSprites = function()
    {
        return __BASEDESTROYED_WELD.getSprites("snow_destroyedweld");
    };
};
Constructor.prototype = __BASEDESTROYED_WELD;
var SNOW_DESTROYEDWELD = new Constructor();
