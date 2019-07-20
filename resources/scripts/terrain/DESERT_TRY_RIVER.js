var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Try River"));
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
        var surroundings = terrain.getSurroundings("DESERT_TRY_RIVER", false, false, GameEnums.Directions_Direct);
        terrain.loadBaseSprite("desert_try_river" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert_try_river";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_river";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_river";
    };
};
Constructor.prototype = TERRAIN;
var DESERT_TRY_RIVER = new Constructor();
