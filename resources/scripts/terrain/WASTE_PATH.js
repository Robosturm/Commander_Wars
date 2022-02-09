var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.baseTerrainId = "WASTE";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASESTREET.loadBase(terrain, "waste_path+style0", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("waste_path+style0")
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        __BASESTREET.loadBaseOverlaySprite("waste_path+style0", terrain, map);
    };
};
Constructor.prototype = __BASESTREET;
var WASTE_PATH = new Constructor();
