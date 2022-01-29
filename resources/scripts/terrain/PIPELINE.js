var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASEPIPELINE.loadBase(terrain, "pipeline", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEPIPELINE.getSprites("pipeline")
    };
};
Constructor.prototype = __BASEPIPELINE;
var PIPELINE = new Constructor();
