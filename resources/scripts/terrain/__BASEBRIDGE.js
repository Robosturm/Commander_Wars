var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.getDefaultPalette = function()
    {
        return "palette_clear";
    };
    this.init = function (terrain)
    {
        terrain.setPalette(Global[terrain.getTerrainID()].getDefaultPalette());
        terrain.setTerrainName(Global[terrain.getTerrainID()].getName(terrain));
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
    this.loadBaseTerrain = function(terrain, currentTerrainID, map, currentPalette)
    {
        if (currentTerrainID === "SEA")
        {
            terrain.loadBaseTerrain("SEA", currentPalette);
        }
        else if (currentTerrainID === "DESERT_TRY_RIVER")
        {
            terrain.loadBaseTerrain("DESERT_TRY_RIVER", currentPalette);
        }
        else if (currentTerrainID === "LAKE")
        {
            terrain.loadBaseTerrain("LAKE", currentPalette);
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
        var variables = terrain.getAnimationVariables();
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
    this.getEditorPlacementSound = function()
    {
        return "placeStreet.wav";
    };
};
Constructor.prototype = TERRAIN;
var __BASEBRIDGE = new Constructor();
