var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.loadBaseSprite = function(terrain)
    {
        __BASEPIPELINE.loadBase(terrain, "waste_pipeline")
    };
    this.getTerrainSprites = function()
    {
        return __BASEPIPELINE.getSprites("waste_pipeline")
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else
        {
            terrain.loadBaseTerrain("WASTE");
        }
    };
};
Constructor.prototype = __BASEPIPELINE;
var WASTE_PIPELINE = new Constructor();
