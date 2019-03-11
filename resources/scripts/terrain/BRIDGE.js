var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Bridge"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("RIVER");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("RIVER,SEA,ROUGH_SEA,REAF,BEACH", false, false, GameEnums.Directions_Direct, false);
        terrain.loadBaseSprite("bridge" + surroundings);
    };
    this.useTerrainAsBaseTerrain = function()
    {
        return true;
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        if ((terrain.getTerrainID() === "SEA") ||
            (terrain.getTerrainID() === "RIVER") ||
            (terrain.getTerrainID() === "BRIDGE"))
        {
            return true;
        }
        else
        {
            return false;
        }

    };
    this.getMiniMapIcon = function()
    {
        return "minimap_bridge";
    };
};
Constructor.prototype = TERRAIN;
var BRIDGE = new Constructor();
