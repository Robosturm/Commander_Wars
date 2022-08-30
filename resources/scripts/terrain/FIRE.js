var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(FIRE.getName());
    };
    this.getName = function()
    {
        return qsTr("Fire");
    };
    this.getDefense = function()
    {
        return 0;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID, map)
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
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else if (currentTerrainID === "WASTE_WASTELAND")
        {
            terrain.loadBaseTerrain("WASTE_WASTELAND");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain, map)
    {
        terrain.loadBaseSprite("fire");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_fire";
    };
    this.getVision = function(player, terrain, map)
    {
        return 5;
    };
    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War, fire make a </r><div c='#00ff00'>5 square </div><r>area around them visible.</r>");
    };
};
Constructor.prototype = TERRAIN;
var FIRE = new Constructor();
