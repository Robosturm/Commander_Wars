var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain)
    {
        __BASERUIN.loadBase(terrain, "ruin");
    };
    this.getTerrainSprites = function()
    {
        return __BASERUIN.getSprites("ruin")
    };
};
Constructor.prototype = __BASERUIN;
var RUIN = new Constructor();
