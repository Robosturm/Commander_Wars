var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(__BASESTREET.getName(terrain));
    };
    this.baseTerrainId = "PLAINS";
    this.getName = function(terrain)
    {
        var baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId
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
        var baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId
        if (baseTerrainId === "DESERT")
        {
            return -20;
        }
        return 0;
    };
    this.getBonusVision = function(unit, terrain)
    {
        var baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId
        if (baseTerrainId === "WASTE")
        {
            return 1;
        }
        return 0;
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
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else
        {
            var baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId
            terrain.loadBaseTerrain(baseTerrainId);
        }
    };

    this.loadBase = function(terrain, spriteId)
    {
        var surroundings = terrain.getSurroundings("STREET,STREET1,BRIDGE,BRIDGE1,DESERT_PATH,DESERT_PATH1,WASTE_PATH",
                                                false, false, GameEnums.Directions_Direct, false, true);
        terrain.loadBaseSprite(spriteId + surroundings);
    };
    this.getMiniMapIcon = function(terrain)
    {
        var baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId
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
        var baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId
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
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        var variables = terrain.getVariables();
        var variable = variables.getVariable("FOREGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 1);
            variable = variables.createVariable("FOREGROUND_ID");
            variable.writeDataInt32(rand);
        }
        else
        {
            rand = variable.readDataInt32();
        }
        var baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId
        if (baseTerrainId === "WASTE")
        {
            return "fore_street+" + rand.toString();
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
            var weatherModifier = TERRAIN.getWeatherModifier();
            return "fore_" + weatherModifier + "street+" + rand.toString();
        }
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var variables = terrain.getVariables();
        var variable = variables.getVariable("BACKGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 2);
            variable = variables.createVariable("BACKGROUND_ID");
            variable.writeDataInt32(rand);
        }
        else
        {
            rand = variable.readDataInt32();
        }
        var baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId
        var id = TERRAIN.getTerrainAnimationId(terrain);
        if (baseTerrainId === "WASTE")
        {
            var weatherModifier = TERRAIN.getWeatherModifier();
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
            var weatherModifier = TERRAIN.getWeatherModifier();
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
};
Constructor.prototype = TERRAIN;
var __BASESTREET = new Constructor();
