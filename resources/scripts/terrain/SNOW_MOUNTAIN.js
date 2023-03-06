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
        __BASEMOUNTAIN.loadBase(terrain, "snow_mountain", map)
    };

    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("snow_mountain");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var SNOW_MOUNTAIN = new Constructor();
