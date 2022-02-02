var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASESTREET.loadBase(terrain, "desert_path+style1", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("desert_path+style1")
    };
};
Constructor.prototype = __BASESTREET;
var DESERT_PATH = new Constructor();
