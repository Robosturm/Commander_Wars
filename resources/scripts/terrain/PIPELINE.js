var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Pipeline"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
		var surroundings = terrain.getSurroundings("PIPELINE,WELD,DESTROYEDWELD", false, false, TERRAIN.Direct);
        if (surroundings === "")
        {
            terrain.loadBaseSprite("pipeline+E+W");
        }
        else
        {
            terrain.loadBaseSprite("pipeline" + surroundings);
        }
    };

};
Constructor.prototype = TERRAIN;
var PIPELINE = new Constructor();
