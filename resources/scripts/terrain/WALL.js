var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Wall"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("WALL", false, false, GameEnums.Directions_Direct);
        if (surroundings === "")
        {
            terrain.loadBaseSprite("wall+E+W");
        }
        else
        {
            terrain.loadBaseSprite("wall" + surroundings);
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_pipeline";
    };
};
Constructor.prototype = TERRAIN;
var WALL = new Constructor();
