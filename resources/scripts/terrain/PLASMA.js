var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.terrainName = qsTr("Plasma");
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
		var surroundings = terrain.getSurroundings("PLASMA,METEOR", false, false, TERRAIN.Direct);
        terrain.loadBaseSprite("plasma" + surroundings);
    };
};
Constructor.prototype = TERRAIN;
var PLASMA = new Constructor();
