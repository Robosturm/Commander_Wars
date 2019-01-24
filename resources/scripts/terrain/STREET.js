var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Street"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("STREET,BRIDGE", false, false, TERRAIN.Direct);
        terrain.loadBaseSprite("street" + surroundings);
    };
};
Constructor.prototype = TERRAIN;
var STREET = new Constructor();
