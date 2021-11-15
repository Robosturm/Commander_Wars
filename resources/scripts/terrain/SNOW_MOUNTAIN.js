var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
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
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("SNOW");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        __BASEMOUNTAIN.loadBase(terrain, "snow_mountain")
    };

    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("snow_mountain");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var SNOW_MOUNTAIN = new Constructor();
