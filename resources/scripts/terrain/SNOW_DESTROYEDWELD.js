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
        __BASEDESTROYED_WELD.loadBase(terrain, "snow_destroyedweld", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEDESTROYED_WELD.getSprites("snow_destroyedweld");
    };
};
Constructor.prototype = __BASEDESTROYED_WELD;
var SNOW_DESTROYEDWELD = new Constructor();
