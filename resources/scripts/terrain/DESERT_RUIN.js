var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain)
    {
        __BASERUIN.loadBase(terrain, "desert_ruin");
    };
    this.getTerrainSprites = function()
    {
        return __BASERUIN.getSprites("desert_ruin");
    };
};
Constructor.prototype = __BASERUIN;
var DESERT_RUIN = new Constructor();
