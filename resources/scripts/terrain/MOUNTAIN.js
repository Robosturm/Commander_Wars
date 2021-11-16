var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain)
    {
        __BASEMOUNTAIN.loadBase(terrain, "mountain")
    };
    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("mountain");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var MOUNTAIN = new Constructor();
