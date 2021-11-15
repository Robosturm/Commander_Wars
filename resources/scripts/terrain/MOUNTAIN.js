var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
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
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        __BASEMOUNTAIN.loadBase(terrain, "mountain")
    };
    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("mountain");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var MOUNTAIN = new Constructor();
