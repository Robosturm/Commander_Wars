var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.terrainName = qsTr("River");
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
		var surroundings = terrain.getSurroundings("RIVER,BRIDGE", false, false, TERRAIN.Direct);
        terrain.loadBaseSprite("river" + surroundings);
    };
};
Constructor.prototype = TERRAIN;
var RIVER = new Constructor();
