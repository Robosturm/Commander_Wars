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
                               qsTr("Snow")],

    getTerrainGroupName : function(item)
    {
        if (item >= 0 && item < terrainGroupNameMapping.length)
        {
            return terrainGroupNameMapping[item];
        }
        return qsTr("Unknown");
    },

    // loader for stuff which needs C++ Support
    init : function (terrain)
    {
    },
    // returns the defense of this terrain
    getDefense : function(terrain)
    {
        return 0;
    },

    getFirerangeModifier : function(terrain, unit)
    {
        return 0;
    },

    getMinFirerangeModifier : function(terrain, unit)
    {
        return 0;
    },

    // called for loading a sub image terrain
    // this has only a visual effect
    loadBaseTerrain : function(terrain, currentTerrainID)
    {
    },
    // called for loading the main sprite
    loadBaseSprite : function(terrain)
    {
    },
    // called for loading an overlay sprite
    loadOverlaySprite : function(terrain)
    {
    },

    getName : function()
    {
        return "";
    },

    // vision created by this field
    getVision : function(player, terrain)
    {
        return -1;
    },

    getBonusVision : function(unit, terrain)
    {
        return 0;
    },

    getVisionHide : function(terrain, player)
    {
        return false;
    },

    getMiniMapIcon : function(terrain)
    {
        return "minimap_plains";
    },

    onDestroyed : function(terrain)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
    },

    getTerrainAnimationBase : function(unit, terrain, defender)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "base_" + weatherModifier + "air";
    },

    getTerrainAnimationForeground : function(unit, terrain, defender)
    {
        var rand = globals.randInt(0, 3);
        var foreground = TERRAIN.getFactoryForeground(terrain);
        var weather = map.getGameRules().getCurrentWeather().getWeatherId();
        if (foreground !== "")
        {
            return foreground;
        }
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "fore_" + weatherModifier +"plains+" + rand.toString();
    },

    getFactoryForeground : function(terrain)
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
                rand = globals.randInt(0, 1);
                return "fore_factory+" + rand.toString();
            }
        }
    },

    getTerrainAnimationId : function(terrain)
    {
        var id = "PLAINS";
        var y = terrain.getY() - 1;
        var x = terrain.getX();
        if (map.onMap(x, y))
        {
            var upTerrain = map.getTerrain(x, y);
            id = upTerrain.getID();
            if (upTerrain.getBuilding() !== null)
            {
                id = "BUILDING";
            }
        }
        return id;
    },

    getTerrainAnimationBackground : function(unit, terrain, defender)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain);
        var weatherModifier = TERRAIN.getWeatherModifier();
        return TERRAIN.getTerrainBackgroundId(id, weatherModifier);
    },

    getWeatherModifier : function()
    {
        var weatherModifier = "";
        var weather = map.getGameRules().getCurrentWeather().getWeatherId();
        if (weather === "WEATHER_SNOW")
        {
            weatherModifier = "snow";
        }
        else if (weather === "WEATHER_RAIN")
        {
            weatherModifier = "rain";
        }
        else if (weather === "WEATHER_DESERT")
        {
            weatherModifier = "desert";
        }
        return weatherModifier;
    },

    getTerrainBackgroundId : function(id, weatherModifier)
    {
        switch (id)
        {
        case "SEA":
        case "BEACH":
        case "FOG":
        case "REAF":
        case "ROUGH_SEA":
            return "back_" + weatherModifier + "planes+sea";
        case "FOREST":
        case "DESERT_FOREST":
        case "SNOW_FOREST":
            return "back_" + weatherModifier + "planes+forest";
        case "SNOW_MOUNTAIN":
        case "DESERT_ROCK":
        case "MOUNTAIN":
            return "back_" + weatherModifier + "planes+mountain";
        case "BUILDING":
            return "back_" + weatherModifier + "planes+town";
        case "STREET":
            return "back_" + weatherModifier + "planes+street";
        case "DESERT_WELD":
        case "SNOW_WELD":
        case "WELD":
        case "PIPELINE":
        case "DESERT_PIPELINE":
        case "SNOW_PIPELINE":
            return "back_" + weatherModifier + "planes+pipe";
        default:
            return "back_" + weatherModifier + "planes";
        }
    },

    getTerrainAnimationMoveSpeed : function(terrain)
    {
        return 0;
    },

    getDescription : function(terrain)
    {
        return "";
    },
    /***********************************************************************************/
    // this section deals with map editor hints
    // these functions have no influence in the game.
    /***********************************************************************************/
    // defines if a terrain can be placed in the editor at a given location
    canBePlaced : function(x, y)
    {
        return true;
    },
    // only bridges use this function for now.
    // all other replace the terrain complete with themself
    // and their base terrain
    useTerrainAsBaseTerrain : function()
    {
        return false;
    },

    getTerrainSprites : function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [];
    },

    startOfTurn : function(terrain)
    {
        // needs to be enable by calling setHasStartOfTurn(true) in the js function init
    },

    // additional offensive bonus for a unit on this field
    getOffensiveFieldBonus : function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckMode)
    {
        return 0;
    },
    //  additional deffensive bonus for a unit on this field
    getDeffensiveFieldBonus : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender, action, luckMode)
    {
        return 0;
    },
};
