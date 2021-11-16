var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain)
    {
        __BASESTREET.loadBase(terrain, "street+style0")
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("street+style0")
    };
};
Constructor.prototype = __BASESTREET;
var STREET1 = new Constructor();
