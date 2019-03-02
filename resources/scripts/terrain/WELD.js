var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Weld"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PIPELINE", false, false, GameEnums.Directions_Direct, false);
        if ((surroundings === ""))
        {
            terrain.loadBaseSprite("weld+E+W");
        }
        else if ((surroundings === "+N+S"))
        {
			
            terrain.loadBaseSprite("weld+N+S");
        }
        else if ((surroundings === "+E+W"))
        {
            terrain.loadBaseSprite("weld+E+W");
        }
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        var surroundings = terrain.getSurroundings("PIPELINE", false, false, GameEnums.Directions_Direct, false);
        if ((surroundings === "+E+W") || (surroundings === "+N+S"))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
};
Constructor.prototype = TERRAIN;
var WELD = new Constructor();
