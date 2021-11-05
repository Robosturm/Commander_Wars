var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(STREET.getName());
    };
    this.getName = function()
    {
        return qsTr("Street");
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
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("STREET,BRIDGE,DESERT_PATH",
                                                   false, false, GameEnums.Directions_Direct, false, true);
        terrain.loadBaseSprite("street" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_street";
    };
    this.getDescription = function()
    {
        return qsTr("Well-surfaced roads provides optimum mobility but little cover.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["street",
                "street+E",
                "street+E+S",
                "street+E+S+W",
                "street+E+W",
                "street+N",
                "street+N+E",
                "street+N+E+S",
                "street+N+E+S+W",
                "street+N+E+W",
                "street+N+S",
                "street+N+S+W",
                "street+N+W",
                "street+S",
                "street+S+W",
                "street+W"];
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
        var foreground = TERRAIN.getFactoryForeground(terrain);
        if (foreground !== "")
        {
            return foreground;
        }
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "fore_" + weatherModifier + "street+" + rand.toString();
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain);
        var weatherModifier = TERRAIN.getWeatherModifier();
        var baseId = terrain.getBaseTerrainID();
        switch (id)
        {
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
        case "SNOW_FOREST":
            return "back_" + weatherModifier + "street+forest";
        default:
            return "back_" + weatherModifier + "street";
        }
    };
};
Constructor.prototype = TERRAIN;
var STREET = new Constructor();
