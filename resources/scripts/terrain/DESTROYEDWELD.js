var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Destroyed Weld"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PIPELINE", false, false, GameEnums.Directions_Direct, false);
        var x = terrain.getX();
        var y = terrain.getY();
        if (map.onMap(x, y + 1))
        {
            var building = map.getTerrain(x, y + 1).getBuilding();
            if (building !== null &&
                building.getBuildingID() === "ZBLACKHOLE_FACTORY" &&
                building.getX() - 1 === x && building.getY() - 4 === y)
            {
                if (surroundings.indexOf("+W") >= 0)
                {
                    surroundings = surroundings.replace("+W", "+S+W");
                }
                else
                {
                    surroundings += "+S";
                }
            }
        }
        if ((surroundings === ""))
        {
            terrain.loadBaseSprite("destroyedweld+E+W");
        }
        else if ((surroundings === "+N+S"))
        {
			
            terrain.loadBaseSprite("destroyedweld+N+S");
        }
        else if ((surroundings === "+E+W"))
        {
            terrain.loadBaseSprite("destroyedweld+E+W");
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
    this.getDefense = function()
    {
        return 1;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plains";
    };
};
Constructor.prototype = TERRAIN;
var DESTROYEDWELD = new Constructor();
