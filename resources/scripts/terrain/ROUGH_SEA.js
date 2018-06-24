var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        // default a terrain has no animation
        terrain.Animated = true;
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("rough_sea+N+E+S+W");
    };
    this.getDefense = function()
    {
        return 2;
    };
};
Constructor.prototype = TERRAIN;
var ROUGH_SEA = new Constructor();
