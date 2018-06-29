var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
		var random = globals.randInt(0, 3);
        if (random > 0)
        {
            terrain.loadBaseSprite("reaf+" + random.toString() + "+N+E+S+W");
        }
        else
        {
            terrain.loadBaseSprite("reaf+N+E+S+W");
        }
    };
    this.getDefense = function()
    {
        return 2;
    };
};
Constructor.prototype = TERRAIN;
var REAF = new Constructor();
