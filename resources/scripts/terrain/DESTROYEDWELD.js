var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESTROYEDWELD.getName());
    };
    this.getName = function()
    {
        return qsTr("Destroyed Weld");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PIPELINE,SNOW_PIPELINE,DESERT_PIPELINE", false, false, GameEnums.Directions_Direct, false);
        var x = terrain.getX();
        var y = terrain.getY();
        if (typeof map !== 'undefined')
        {
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
        var surroundings = terrain.getSurroundings("PIPELINE,DESERT_PIPELINE,SNOW_PIPELINE", false, false, GameEnums.Directions_Direct, false);
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
    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain.");
    };
    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["destroyedweld+E+W",
                "destroyedweld+N+S"];
    };
};
Constructor.prototype = TERRAIN;
var DESTROYEDWELD = new Constructor();
