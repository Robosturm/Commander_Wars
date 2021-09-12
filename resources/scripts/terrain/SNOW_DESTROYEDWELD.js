var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(SNOW_DESTROYEDWELD.getName());
    };
    this.getName = function()
    {
        return qsTr("Snowy Destroyed Weld");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else
        {
            terrain.loadBaseTerrain("SNOW");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PIPELINE,SNOW_PIPELINE,DESERT_PIPELINE", false, false, GameEnums.Directions_Direct, true);
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
            terrain.loadBaseSprite("snow_destroyedweld+E+W");
        }
        else if ((surroundings === "+N+S"))
        {
			
            terrain.loadBaseSprite("snow_destroyedweld+N+S");
        }
        else if ((surroundings === "+E+W"))
        {
            terrain.loadBaseSprite("snow_destroyedweld+E+W");
        }
        else
        {
            terrain.loadBaseSprite("snow_destroyedweld+E+W");
        }
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        var surroundings = terrain.getSurroundings("PIPELINE,DESERT_PIPELINE,SNOW_PIPELINE", false, false, GameEnums.Directions_Direct, true);
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
        return qsTr("Snowy terrain reduces rough to cross.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["snow_destroyedweld+E+W",
                "snow_destroyedweld+N+S"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 3);
        return "fore_snowplains+" + rand.toString();
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain);
        return TERRAIN.getTerrainAnimationBackground(id, "snow");
    };
};
Constructor.prototype = TERRAIN;
var SNOW_DESTROYEDWELD = new Constructor();
