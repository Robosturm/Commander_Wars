var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.getDefaultPalette = function()
    {
        return "palette_snow";
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEWELD.loadBase(terrain, "snow_weld", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEWELD.getSprites("snow_weld")
    };
    this.onDestroyed = function(terrain, map)
    {
        __BASEWELD.onBaseDestroyed(terrain, "SNOW_DESTROYEDWELD", map);
    };
};
Constructor.prototype = __BASEWELD;
var SNOW_WELD = new Constructor();
