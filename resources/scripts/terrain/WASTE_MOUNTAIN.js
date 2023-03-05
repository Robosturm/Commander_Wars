var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.getDefaultPalette = function()
    {
        return "palette_waste";
    };
    this.baseTerrainId = "WASTE";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEMOUNTAIN.loadBase(terrain, "waste_rock", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("waste_rock");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var WASTE_MOUNTAIN = new Constructor();
