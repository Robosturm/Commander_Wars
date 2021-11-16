var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.baseTerrainId = "SNOW";

    this.loadBaseSprite = function(terrain)
    {
        __BASERUIN.loadBase(terrain, "snow_ruin");
    };
    this.getTerrainSprites = function()
    {
        return __BASERUIN.getSprites("snow_ruin");
    };
};
Constructor.prototype = __BASERUIN;
var SNOW_RUIN = new Constructor();
