var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.terrainName = qsTr("Street");
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("RIVER");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("RIVER,SEA,ROUGH_SEA,REAF,BEACH", false, false, TERRAIN.Direct, false);
        terrain.loadBaseSprite("bridge" + surroundings);
    };
    this.useTerrainAsBaseTerrain = function()
    {
        return true;
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        if ((terrain.terrainID === "SEA") ||
            (terrain.terrainID === "RIVER") ||
            (terrain.terrainID === "BRIDGE"))
        {
            return true;
        }
        else
        {
            return false;
        }

    }
};
Constructor.prototype = TERRAIN;
var BRIDGE = new Constructor();
