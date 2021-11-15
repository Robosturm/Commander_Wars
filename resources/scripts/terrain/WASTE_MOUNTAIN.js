var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
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
    this.loadBaseSprite = function(terrain)
    {
        __BASEMOUNTAIN.loadBase(terrain, "waste_rock")
    };
    this.getTerrainSprites = function()
    {
        return __BASEMOUNTAIN.getSprites("waste_rock");
    };
};
Constructor.prototype = __BASEMOUNTAIN;
var WASTE_MOUNTAIN = new Constructor();
