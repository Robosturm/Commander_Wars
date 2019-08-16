var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESERT_TRY_RIVER.getName());
    };
    this.getName = function()
    {
        return qsTr("Try River");
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
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
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

    this.getDescription = function()
    {
        return qsTr("Once a river. Now an empty Riverbed that can be crossed by most units doesn't provide cover. It reduces the firerange of indirect units by 1.");
    };
};
Constructor.prototype = TERRAIN;
var DESERT_TRY_RIVER = new Constructor();
