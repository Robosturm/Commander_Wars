var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain)
    {
        __BASEMOUNTAIN.loadBase(terrain, "snow_mountain")
    };

    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("snow_mountain");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var SNOW_MOUNTAIN = new Constructor();
