var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        if (terrain.getPalette() === "")
        {
            terrain.setPalette(Global[terrain.getTerrainID()].getDefaultPalette());
        }
        terrain.setTerrainName(Global[terrain.getTerrainID()].getName(terrain));
    };
    this.baseTerrainId = "PLAINS";
    this.getName = function(terrain = null)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return qsTr("Waste Street");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Snowy Street");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Path");
        }
        else
        {
            return qsTr("Street");
        }
    };
    this.getOffensiveFieldBonus = function(terrain, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender, action, luckMode)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "DESERT")
        {
            return -20;
        }
        return 0;
    };
    this.getBonusVision = function(unit, terrain)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return 1;
        }
        return 0;
    };

    this.loadBaseTerrain = function(terrain, currentTerrainID, map, currentPalette)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW", currentPalette);
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT", currentPalette);
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE", currentPalette);
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS", currentPalette);
        }
        else
        {
            var baseTerrainId = ""
            if (terrain !== null)
            {
                baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
            }
            terrain.loadBaseTerrain(baseTerrainId);
        }
    };

    this.loadBase = function(terrain, spriteId, map)
    {
        var surroundings = __BASESTREET.getStreetSurrounding(terrain);
        terrain.loadBaseSprite(spriteId + surroundings);
    };

    this.getStreetSurrounding = function(terrain)
    {
        var surroundings = terrain.getSurroundings("STREET,STREET1,BRIDGE,BRIDGE1,BRIDGE2,SNOW_STREET,DESERT_PATH,DESERT_PATH1,WASTE_PATH",
                                                false, false, GameEnums.Directions_Direct, false, true);
        return surroundings;
    };

    this.getMiniMapIcon = function(terrain)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return "minimap_street";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "minimap_street";
        }
        else if (baseTerrainId === "DESERT")
        {
            return "minimap_street";
        }
        else
        {
            return "minimap_street";
        }
    };
    this.getDescription = function(terrain)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return qsTr("Well-surfaced roads provides optimum mobility but little cover.");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Well-surfaced roads provides optimum mobility but little cover.");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Kinda well-surfaced path through the desert provides optimum mobility but little cover. It reduces the firepower of units by 20%");
        }
        else
        {
            return qsTr("Well-surfaced roads provides optimum mobility but little cover.");
        }
    };
    this.getSprites = function(spriteId)
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [spriteId,
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
                spriteId + "+W"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        var variables = terrain.getAnimationVariables();
        var variable = variables.getVariable("FOREGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 1, true);
            variable = variables.createVariable("FOREGROUND_ID");
            variable.writeDataInt32(rand);
        }
        else
        {
            rand = variable.readDataInt32();
        }
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return "";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "fore_snowstreet+" + rand.toString();
        }
        else if (baseTerrainId === "DESERT")
        {
            return "fore_desertstreet+" + rand.toString();
        }
        else
        {
            var weatherModifier = TERRAIN.getWeatherModifier(map);
            return "fore_" + weatherModifier + "street+" + rand.toString();
        }
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var variables = terrain.getAnimationVariables();
        var variable = variables.getVariable("BACKGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 2, true);
            variable = variables.createVariable("BACKGROUND_ID");
            variable.writeDataInt32(rand);
        }
        else
        {
            rand = variable.readDataInt32();
        }
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        var id = TERRAIN.getTerrainAnimationId(terrain, map);
        if (baseTerrainId === "WASTE")
        {
            var weatherModifier = TERRAIN.getWeatherModifier(map);
            if (weatherModifier === "")
            {
                weatherModifier = TERRAIN.getTerrainWeatherModifier(terrain);
            }
            return __BASESTREET.getStreetAnimationBackground(id, weatherModifier);
        }
        else if (baseTerrainId === "SNOW")
        {
            return __BASESTREET.getStreetAnimationBackground(id, "SNOW");
        }
        else if (baseTerrainId === "DESERT")
        {
            return __BASESTREET.getStreetAnimationBackground(id, "DESERT");
        }
        else
        {
            var weatherModifier = TERRAIN.getWeatherModifier(map);
            return __BASESTREET.getStreetAnimationBackground(id, weatherModifier);
        }
    };
    this.getStreetAnimationBackground = function(id, weatherModifier)
    {
        switch (id)
        {
        case "LAKE":
        case "SEA":
        case "BEACH":
        case "FOG":
        case "REAF":
        case "ROUGH_SEA":
            return "back_" + weatherModifier + "street+sea";
        case "SNOW_MOUNTAIN":
        case "DESERT_ROCK":
        case "MOUNTAIN":
            return "back_" + weatherModifier + "street+mountain";
        case "BUILDING":
            return "back_" + weatherModifier + "street+town";
        case "DESERT_WELD":
        case "SNOW_WELD":
        case "WELD":
        case "PIPELINE":
        case "DESERT_PIPELINE":
        case "SNOW_PIPELINE":
        case "WASTE_PIPELINE":
        case "ZWELD_N_S":
        case "ZWELD_E_W":
        case "ZSNOWWELD_N_S":
        case "ZSNOWWELD_E_W":
        case "ZDESERTWELD_N_S":
        case "ZDESERTWELD_E_W":
            return "back_" + weatherModifier + "street+pipe";
        case "FOREST":
        case "FOREST1":
        case "FOREST2":
        case "FOREST3":
        case "DESERT_FOREST":
        case "DESERT_FOREST1":
        case "SNOW_FOREST":
        case "SNOW_FOREST1":
        case "SNOW_FOREST2":
            return "back_" + weatherModifier + "street+forest";
        default:
            return "back_" + weatherModifier + "street";
        }
    };

    this.loadBaseOverlaySprite = function(spriteId, terrain, map)
    {
        var x = terrain.getX();
        var y = terrain.getY();
        var highTerrain = terrain.getSurroundings(TERRAIN.getHighTerrains(), true, false, GameEnums.Directions_West, false);
        if (map.onMap(x - 1, y))
        {
            var building = map.getTerrain(x - 1, y).getBuilding();
            if (building !== null &&
                building.getBuildingWidth() === 1 &&
                building.getBuildingHeigth() === 1)
            {
                highTerrain = "+W";
            }
        }
        if (highTerrain !== "")
        {
            var surroundings = terrain.getTerrainSpriteName();
            terrain.loadOverlaySprite(surroundings + "+shadow", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_West, ""));
        }
    };

    this.getBaseOverlayTerrainSprites = function(spriteId)
    {
        return [spriteId + "+shadow",
                spriteId + "+E+shadow",
                spriteId + "+E+S+shadow",
                spriteId + "+E+S+W+shadow",
                spriteId + "+E+W+shadow",
                spriteId + "+N+shadow",
                spriteId + "+N+E+shadow",
                spriteId + "+N+E+S+shadow",
                spriteId + "+N+E+S+W+shadow",
                spriteId + "+N+E+W+shadow",
                spriteId + "+N+S+shadow",
                spriteId + "+N+S+W+shadow",
                spriteId + "+N+W+shadow",
                spriteId + "+S+shadow",
                spriteId + "+S+W+shadow",
                spriteId + "+W+shadow",]
    };
    this.getEditorPlacementSound = function()
    {
        return "placeStreet.wav";
    };
};
Constructor.prototype = TERRAIN;
var __BASESTREET = new Constructor();
