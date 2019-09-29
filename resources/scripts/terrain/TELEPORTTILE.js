var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(TELEPORTTILE.getName());
    };

    this.getName = function()
    {
        return qsTr("Teleport Tile");
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
        var surroundings = terrain.getSurroundings("TELEPORTTILE", false, false, GameEnums.Directions_Direct, false, true);
        terrain.loadBaseSprite("teleporttile" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_teleport";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_sea";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_sea";
    };
    this.getDescription = function()
    {
        return qsTr("A mysterious tile that costs 0MP to move over.");
    };
};
Constructor.prototype = TERRAIN;
var TELEPORTTILE = new Constructor();
