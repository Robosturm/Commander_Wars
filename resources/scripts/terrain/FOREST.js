var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.terrainName = qsTr("Forest");
    };
    this.getDefense = function()
    {
        return 3;
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 1);
        terrain.loadBaseSprite("forest+" + random.toString());
    };
};
Constructor.prototype = TERRAIN;
var FOREST = new Constructor();
