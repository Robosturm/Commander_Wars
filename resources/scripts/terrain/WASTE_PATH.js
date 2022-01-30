var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.baseTerrainId = "WASTE";
    this.loadBaseSprite = function(terrain)
    {
        __BASESTREET.loadBase(terrain, "waste_path+style0")
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("waste_path+style0")
    };
};
Constructor.prototype = __BASESTREET;
var WASTE_PATH = new Constructor();
