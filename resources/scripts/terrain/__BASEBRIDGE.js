var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(__BASEBRIDGE.getName(terrain));
    };
    this.canBePlacedBaseId = function(x, y, map, baseId)
    {
        var terrain = map.getTerrain(x, y);
        if ((terrain.getTerrainID() === "SEA") ||
            (terrain.getTerrainID() === "LAKE") ||
            (terrain.getTerrainID() === "RIVER") ||
            (terrain.getTerrainID() === "DESERT_TRY_RIVER") ||
            (terrain.getTerrainID() === baseId))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID, map)
    {
        if (currentTerrainID === "SEA")
        {
            terrain.loadBaseTerrain("SEA");
        }
        else if (currentTerrainID === "DESERT_TRY_RIVER")
        {
            terrain.loadBaseTerrain("DESERT_TRY_RIVER");
        }
        else if (currentTerrainID === "LAKE")
        {
            terrain.loadBaseTerrain("LAKE");
        }
        else
        {
            terrain.loadBaseTerrain("RIVER");
        }
    };
    this.loadSprite = function(terrain, style, map)
    {
        var surroundings = terrain.getSurroundings("RIVER,LAKE,SEA,ROUGH_SEA,REAF,BEACH,FOG,DESERT_TRY_RIVER", false, false, GameEnums.Directions_Direct, false);
        terrain.loadBaseSprite(style + surroundings);
    };
    this.useTerrainAsBaseTerrain = function()
    {
        return true;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_bridge";
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return TERRAIN.getFactoryForeground(terrain, map);
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var variables = terrain.getVariables();
        var variable = variables.getVariable("BACKGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 1);
            variable = variables.createVariable("BACKGROUND_ID");
            variable.writeDataInt32(rand);
        }
        else
        {
            rand = variable.readDataInt32();
        }
        return __BASEBRIDGE.getTerrainAnimationBackgroundRand(unit, terrain, defender, map, rand);
    }


    this.getTerrainAnimationBackgroundRand = function(unit, terrain, defender, map, rand)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain, map);
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        if (weatherModifier === "")
        {
            weatherModifier = TERRAIN.getTerrainWeatherModifier(terrain);
        }
        switch (id)
        {
            case "LAKE":
            case "SEA":
            case "BEACH":
            case "FOG":
            case "REAF":
            case "ROUGH_SEA":
            {
                return "back_" + weatherModifier + "bridge+sea"
            }
            default:
            {
                return "back_" + weatherModifier + "bridge+" + rand.toString();
            }
        }
    };
    this.getDescription = function()
    {
        return qsTr("Bridge over river and seas which allows ground units to cross. Bridges over sea also allows naval units to cross the field.");
    };

    this.getSprites = function(spriteId)
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [spriteId + "",
                spriteId + "+E",
                spriteId + "+E+S",
                spriteId + "+E+S+W",
                spriteId + "+E+W",
                spriteId + "+N",
                spriteId + "+N+E",
                spriteId + "+N+E+S",
                spriteId + "+N+E+S+W",
                spriteId + "+N+E+W",
                spriteId + "+N+S",
                spriteId + "+N+S+W",
                spriteId + "+N+W",
                spriteId + "+S",
                spriteId + "+S+W",
                spriteId + "+W",];
    };

};
Constructor.prototype = TERRAIN;
var __BASEBRIDGE = new Constructor();
