var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.baseTerrainId = "SNOW";
    this.loadBaseSprite = function(terrain)
    {
        __BASEPIPELINE.loadBase(terrain, "snow_pipeline")
    };
    this.getTerrainSprites = function()
    {
        return __BASEPIPELINE.getSprites("snow_pipeline")
    };
};
Constructor.prototype = __BASEPIPELINE;
var SNOW_PIPELINE = new Constructor();
