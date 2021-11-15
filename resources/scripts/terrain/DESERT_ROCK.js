var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
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
    this.loadBaseSprite = function(terrain)
    {
        __BASEMOUNTAIN.loadBase(terrain, "desert_rock")
    };

    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("desert_rock");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var DESERT_ROCK = new Constructor();
