var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.terrainName = qsTr("Destroyed Weld");
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
		var surroundings = terrain.getSurroundings("PIPELINE,WELD,DESTROYEDWELD", false, false, TERRAIN.Direct);
        if ((surroundings === "") || (surroundings.includes("+E") && surroundings.includes("+W")))
        {
            terrain.loadBaseSprite("destroyedweld+E+W");
        }
        else if ((surroundings === "") || (surroundings.includes("+N") && surroundings.includes("+S")))
        {
			
            terrain.loadBaseSprite("destroyedweld+N+S");
        }
        else
        {
            terrain.loadBaseSprite("destroyedweld+E+W");
        }
    };
};
Constructor.prototype = TERRAIN;
var DESTROYEDWELD = new Constructor();
