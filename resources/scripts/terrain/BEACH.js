var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.terrainName = qsTr("Beach");
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
		var surroundings = terrain.getSurroundings("SEA", true, true, TERRAIN.Direct);
        terrain.loadBaseSprite("beach" + surroundings);
    };
    this.loadOverlaySprite = function(terrain)
    {
    };
};
Constructor.prototype = TERRAIN;
var BEACH = new Constructor();
