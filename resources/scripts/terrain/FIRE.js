var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Fire"));
    };
    this.getDefense = function()
    {
        return 0;
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
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("fire");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_fire";
    };
    this.getVision = function()
    {
        return 3;
    };
};
Constructor.prototype = TERRAIN;
var FIRE = new Constructor();
