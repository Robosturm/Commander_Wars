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
		var surroundings = terrain.getSurroundings("RIVER,SEA", false, false, TERRAIN.Direct);
        terrain.loadBaseSprite("bridge" + surroundings);
    };
};
Constructor.prototype = TERRAIN;
var BRIDGE = new Constructor();
