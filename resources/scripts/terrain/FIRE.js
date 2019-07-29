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
        else if (currentTerrainID === "WASTELAND")
        {
            terrain.loadBaseTerrain("WASTELAND");
        }
        else if (currentTerrainID === "DESERT_WASTELAND")
        {
            terrain.loadBaseTerrain("DESERT_WASTELAND");
        }
        else if (currentTerrainID === "SNOW_WASTELAND")
        {
            terrain.loadBaseTerrain("SNOW_WASTELAND");
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
        return 5;
    };
    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain.");
    };
};
Constructor.prototype = TERRAIN;
var FIRE = new Constructor();
