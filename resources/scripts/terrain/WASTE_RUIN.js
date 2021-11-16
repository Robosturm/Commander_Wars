var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.baseTerrainId = "WASTE";
    this.loadBaseSprite = function(terrain)
    {
        __BASERUIN.loadBase(terrain, "waste_ruin");
    };
    this.getTerrainSprites = function()
    {
        return __BASERUIN.getSprites("waste_ruin");
    };
};
Constructor.prototype = __BASERUIN;
var WASTE_RUIN = new Constructor();
