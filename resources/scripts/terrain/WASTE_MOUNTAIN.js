var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.baseTerrainId = "WASTE";
    this.loadBaseSprite = function(terrain)
    {
        __BASEMOUNTAIN.loadBase(terrain, "waste_rock")
    };
    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("waste_rock");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var WASTE_MOUNTAIN = new Constructor();
