var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };

    this.loadBaseSprite = function(terrain)
    {
        __BASESTREET.loadBase(terrain, "waste_path+style0")
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("waste_path+style0")
    };

    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else
        {
            terrain.loadBaseTerrain("WASTE");
        }
    };
};
Constructor.prototype = __BASESTREET;
var WASTE_PATH = new Constructor();
