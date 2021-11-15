var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };

    this.loadBaseSprite = function(terrain)
    {
        __BASESTREET.loadBase(terrain, "desert_path+style1")
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("desert_path+style1")
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
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("DESERT");
        }
    };
};
Constructor.prototype = __BASESTREET;
var DESERT_PATH = new Constructor();
