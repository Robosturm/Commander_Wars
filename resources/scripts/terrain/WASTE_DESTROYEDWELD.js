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
        __BASEDESTROYED_WELD.loadBase(terrain, "waste_destroyedweld", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEDESTROYED_WELD.getSprites("waste_destroyedweld");
    };
};
Constructor.prototype = __BASEDESTROYED_WELD;
var WASTE_DESTROYEDWELD = new Constructor();
