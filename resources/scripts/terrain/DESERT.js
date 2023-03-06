var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    this.getDefaultPalette = function()
    {
        return "palette_desert";
    };
    this.init = function (terrain)
    {
        terrain.setPalette(DESERT.getDefaultPalette());
        terrain.setTerrainName(DESERT.getName());
    };

    this.getName = function()
    {
        return qsTr("Desert");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        var random = globals.randInt(0, 140);
        if (random >= 8)
        {
            terrain.loadBaseSprite("desert+0");
        }
        else
        {
            terrain.loadBaseSprite("desert+" + random.toString());
        }
    };
    this.getOffensiveFieldBonus = function(terrain, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender, action, luckMode)
    {
        return -20;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert";
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        var surroundingsPlains = terrain.getSurroundings("PLAINS", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsPlains.includes("+N"))
        {
            terrain.loadOverlaySprite("plains+N", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_North, "PLAINS"));
        }
        if (surroundingsPlains.includes("+E"))
        {
            terrain.loadOverlaySprite("plains+E", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_East, "PLAINS"));
        }
        if (surroundingsPlains.includes("+S"))
        {
            terrain.loadOverlaySprite("plains+S", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_South, "PLAINS"));
        }
        if (surroundingsPlains.includes("+W"))
        {
            terrain.loadOverlaySprite("plains+W", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_West, "PLAINS"));
        }
        var surroundingsSnow = terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow.includes("+N"))
        {
            terrain.loadOverlaySprite("snow+N", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_North, "SNOW"));
        }
        if (surroundingsSnow.includes("+E"))
        {
            terrain.loadOverlaySprite("snow+E", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_East, "SNOW"));
        }
        if (surroundingsSnow.includes("+S"))
        {
            terrain.loadOverlaySprite("snow+S", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_South, "SNOW"));
        }
        if (surroundingsSnow.includes("+W"))
        {
            terrain.loadOverlaySprite("snow+W", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_West, "SNOW"));
        }

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
            terrain.loadOverlaySprite("shadow_desert");
        }
    };

    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain, but it reduces the firepower of units by 20%.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["desert+0",
                "desert+1",
                "desert+2",
                "desert+3",
                "desert+4",
                "desert+5",
                "desert+6",
                "desert+7"];
    };

    this.getOverlayTerrainSprites = function(map)
    {
        return ["shadow_desert",
                "plains+N",
                "plains+E",
                "plains+S",
                "plains+W",
                "snow+N",
                "snow+E",
                "snow+S",
                "snow+W",]
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return TERRAIN.getFactoryForeground(terrain, map);
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain, map);
        switch (id)
        {
        case "SEA":
        case "LAKE":
        case "BEACH":
        case "FOG":
        case "REAF":
        case "ROUGH_SEA":
            return "back_desertplanes+sea";
        case "FOREST":
        case "FOREST1":
        case "FOREST2":
        case "FOREST3":
        case "DESERT_FOREST":
        case "DESERT_FOREST1":
        case "SNOW_FOREST":
        case "SNOW_FOREST1":
        case "SNOW_FOREST2":
            return "back_desertplanes+forest";
        case "SNOW_MOUNTAIN":
        case "DESERT_ROCK":
        case "MOUNTAIN":
            return "back_desertplanes+mountain";
        case "BUILDING":
            return "back_desertplanes+town";
        case "SNOW_STREET":
        case "STREET":
        case "STREET1":
        case "WASTE_PATH":
        case "DESERT_PATH":
        case "DESERT_PATH1":
            return "back_desertplanes+street";
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
            return "back_desertplanes+pipe";
        default:
            return "back_desertplanes";
        }
    };
};
Constructor.prototype = TERRAIN;
var DESERT = new Constructor();
