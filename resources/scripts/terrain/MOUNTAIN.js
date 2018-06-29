var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
    };
    this.getDefense = function()
    {
        return 4;
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 1);
        terrain.loadBaseSprite("mountain+" + random.toString());
    };
};
Constructor.prototype = TERRAIN;
var MOUNTAIN = new Constructor();
