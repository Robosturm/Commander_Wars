var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain)
    {
        __BASEMOUNTAIN.loadBase(terrain, "desert_rock")
    };

    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("desert_rock");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var DESERT_ROCK = new Constructor();
