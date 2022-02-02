var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASESTREET.loadBase(terrain, "snow_street+style0", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("snow_street+style0")
    };
};
Constructor.prototype = __BASESTREET;
var SNOW_STREET = new Constructor();
