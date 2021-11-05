var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESERT_DESTROYEDWELD.getName());
    };
    this.getName = function()
    {
        return qsTr("Desert Destroyed Weld");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("DESERT");
        }
    };
    this.getOffensiveFieldBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender, action, luckMode)
    {
        return -20;
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
            terrain.loadBaseSprite("desert_destroyedweld+E+W");
        }
        else if ((surroundings === "+N+S"))
        {

            terrain.loadBaseSprite("desert_destroyedweld+N+S");
        }
        else if ((surroundings === "+E+W"))
        {
            terrain.loadBaseSprite("desert_destroyedweld+E+W");
        }
        else
        {
            terrain.loadBaseSprite("desert_destroyedweld+E+W");
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
        return qsTr("Ground units move easily on this terrain, but it reduces the firepower of units by 20%");
    };
    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["desert_destroyedweld+E+W",
                "desert_destroyedweld+N+S"];
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender)
    {
        return TERRAIN.getFactoryForeground(terrain);
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain);
        switch (id)
        {
        case "DESERT_WELD":
        case "SNOW_WELD":
        case "WELD":
        case "PIPELINE":
        case "DESERT_PIPELINE":
        case "SNOW_PIPELINE":
        case "ZWELD_N_S":
        case "ZWELD_E_W":
        case "ZSNOWWELD_N_S":
        case "ZSNOWWELD_E_W":
        case "ZDESERTWELD_N_S":
        case "ZDESERTWELD_E_W":
            return "back_desert+pipe";
        default:
            return "back_desert";
        }
    };
};
Constructor.prototype = TERRAIN;
var DESERT_DESTROYEDWELD = new Constructor();
