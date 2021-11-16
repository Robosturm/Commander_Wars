var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain)
    {
        __BASEPIPELINE.loadBase(terrain, "pipeline")
    };
    this.getTerrainSprites = function()
    {
        return __BASEPIPELINE.getSprites("pipeline")
    };
};
Constructor.prototype = __BASEPIPELINE;
var PIPELINE = new Constructor();
