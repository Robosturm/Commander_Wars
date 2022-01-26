// this is the base class for terrain
var TERRAIN =
{
    // search type filter for overlay
    getTerrainGroup : function()
    {
        // terrain groups for editor selection ordering
        // 0 = sea tiles
        // 1 = normal ground tiles
        // 2 = desert tiles
        // 3 = snow tiles
        // everthing else is open to use
        return 0;
    },
    getTerrainGroupSort : function()
    {
        return [1,
                0,
                2,
                3,];
    },
    terrainGroupNameMapping = [qsTr("Sea"),
                               qsTr("Normal"),
                               qsTr("Desert"),
                               qsTr("Snow"),
                               qsTr("Waste")],

    getTerrainGroupName : function(item)
    {
        if (item >= 0 && item < terrainGroupNameMapping.length)
        {
            return terrainGroupNameMapping[item];
        }
        return qsTr("Unknown");
    },

    // loader for stuff which needs C++ Support
    init : function (terrain, map)
    {
    },
    // returns the defense of this terrain
    getDefense : function(terrain, map)
    {
        return 0;
    },

    getFirerangeModifier : function(terrain, unit, map)
    {
        return 0;
    },

    getMinFirerangeModifier : function(terrain, unit, map)
    {
        return 0;
    },

    // called for loading a sub image terrain
    // this has only a visual effect
    loadBaseTerrain : function(terrain, currentTerrainID, map)
    {
    },
    
    loadBaseSprite : function(terrain, map)
    {
    },
    // called for loading an overlay sprite
    loadOverlaySprite : function(terrain, map)
    {
    },

    getName : function()
    {
        return "";
    },

    // vision created by this field
    getVision : function(player, terrain, map)
    {
        return -1;
    },

    getBonusVision : function(unit, terrain, map)
    {
        return 0;
    },

    getVisionHide : function(terrain, player, map)
    {
        return false;
    },

    getMiniMapIcon : function(terrain, map)
    {
        return "minimap_plains";
    },

    onDestroyed : function(terrain, map)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
    },

    getTerrainAnimationBase : function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "base_" + weatherModifier + "air";
    },

    getTerrainAnimationForeground : function(unit, terrain, defender, map)
    {
        var variables = terrain.getVariables();
        var variable = variables.getVariable("FOREGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 3);
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
        return "fore_" + weatherModifier + "plains+" + rand.toString();
    },

    getFactoryForeground : function(terrain, map)
    {
        var y = terrain.getY() - 1;
        var x = terrain.getX();
        if (map.onMap(x, y))
        {
            var upTerrain = map.getTerrain(x, y);
            var upBuilding = upTerrain.getBuilding();
            if (upBuilding !== null &&
                upBuilding.getBuildingID() === "ZBLACKHOLE_FACTORY")
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
                return "fore_factory+" + rand.toString();
            }
        }
        return "";
    },

    getTerrainAnimationId : function(terrain, map)
    {
        var id = "PLAINS";
        var y = terrain.getY() - 1;
        var x = terrain.getX();
        if (map.onMap(x, y))
        {
            var upTerrain = map.getTerrain(x, y);
            id = upTerrain.getID();
            if (id === "ZWELD_E_W" ||
                id === "ZWELD_N_S")
            {
                id = "PIPELINE";
            }
            else if (upTerrain.getBuilding() !== null)
            {
                id = "BUILDING";
            }
        }
        return id;
    },

    getTerrainAnimationBackground : function(unit, terrain, defender, map)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain);
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return TERRAIN.getTerrainBackgroundId(id, weatherModifier);
    },

    weatherData :   [["weather_1sun",         [Qt.point(0, 0),    "",        ""]],
                     ["weather_snow",         [Qt.point(-1, 1),   "snow",    "over_snow"]],
                     ["weather_rain",         [Qt.point(-2, 6),   "rain",    "over_rain"]],
                     ["weather_sandstorm",    [Qt.point(9, 3),    "desert",  "over_sandstorm"]],
                     ["weather_mist",         [Qt.point(0, 0),    "",        ""]],],

    getWeatherModifier : function(map)
    {
        var weatherModifier = "";
        if (map !== null)
        {
            var weather     = map.getGameRules().getCurrentWeather().getWeatherId();
            var data        = Global.getDataFromTable(weather, TERRAIN.weatherData);
            weatherModifier = data[1];
        }
        return weatherModifier;
    },

    getWeatherOverlayId : function(terrain, map)
    {
        var overlay = "";
        if (map !== null)
        {
            var weather     = map.getGameRules().getCurrentWeather().getWeatherId();
            var data        = Global.getDataFromTable(weather, TERRAIN.weatherData);
            overlay         = data[2];
        }
        return overlay;
    },

    getWeatherOverlaySpeed : function(terrain, map)
    {
        var speed = Qt.point(0, 0);
        if (map !== null)
        {
            var weather     = map.getGameRules().getCurrentWeather().getWeatherId();
            var data        = Global.getDataFromTable(weather, TERRAIN.weatherData);
            speed           = data[0];
        }
        return speed;
    },

    isPipeline : function(terrainId)
    {
        switch (terrainId)
        {
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
            return true;
        default:
            return false;
        }
    },

    getTerrainBackgroundId : function(id, weatherModifier, pipe = false)
    {
        switch (id)
        {
        case "SEA":
        case "LAKE":
        case "BEACH":
        case "FOG":
        case "REAF":
        case "ROUGH_SEA":
            return "back_" + weatherModifier + "planes+sea";
        case "FOREST":
        case "FOREST1":
        case "FOREST2":
        case "FOREST3":
        case "DESERT_FOREST":
        case "DESERT_FOREST1":
        case "SNOW_FOREST":
        case "SNOW_FOREST1":
        case "SNOW_FOREST2":
            return "back_" + weatherModifier + "planes+forest";
        case "SNOW_MOUNTAIN":
        case "DESERT_ROCK":
        case "MOUNTAIN":
            return "back_" + weatherModifier + "planes+mountain";
        case "BUILDING":
            return "back_" + weatherModifier + "planes+town";
        case "SNOW_STREET":
        case "STREET":
        case "STREET1":
        case "WASTE_PATH":
        case "DESERT_PATH":
        case "DESERT_PATH1":
            return "back_" + weatherModifier + "planes+street";
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
            if (pipe)
            {
                return "back_" + weatherModifier + "planes";
            }
            else
            {
                return "back_" + weatherModifier + "planes+pipe";
            }
        default:
            return "back_" + weatherModifier + "planes";
        }
    },

    getTerrainAnimationMoveSpeed : function(terrain, map)
    {
        return 0;
    },

    getMovementcostModifier : function(terrain, x, y, curX, curY, map)
    {
        return 0;
    },

    getDescription : function(terrain, map)
    {
        return "";
    },
    /***********************************************************************************/
    // this section deals with map editor hints
    // these functions have no influence in the game.
    /***********************************************************************************/
    // defines if a terrain can be placed in the editor at a given location
    canBePlaced : function(x, y, map)
    {
        return true;
    },
    // only bridges use this function for now.
    // all other replace the terrain complete with themself
    // and their base terrain
    useTerrainAsBaseTerrain : function(map)
    {
        return false;
    },

    getTerrainSprites : function(map)
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [];
    },

    startOfTurn : function(terrain, map)
    {
        // needs to be enable by calling setHasStartOfTurn(true) in the js function init
    },

    // additional offensive bonus for a unit on this field
    getOffensiveFieldBonus : function(terrain, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckMode, map)
    {
        return 0;
    },
    //  additional deffensive bonus for a unit on this field
    getDeffensiveFieldBonus : function(terrain, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckMode, map)
    {
        return 0;
    },

    // the tiles considered for flowing
    getFlowTiles : function(map)
    {
        return [];
    },
    // gets called with a terrain pfs in order to create the flow sprites
    updateFlowSprites : function(terrain, pPfs, map)
    {
    },
    isLoadingTile :  function(terrain, map)
    {
        // hint for the ai to try to move away if it can't do anything with a unit
        return false;
    },
};
