var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.baseTerrainId = "DESERT";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEPIPELINE.loadBase(terrain, "desert_pipeline", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEPIPELINE.getSprites("desert_pipeline")
    };
};
Constructor.prototype = __BASEPIPELINE;
var DESERT_PIPELINE = new Constructor();
