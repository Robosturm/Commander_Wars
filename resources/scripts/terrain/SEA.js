var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    this.getDefaultPalette = function()
    {
        return "palette_clear";
    };
    this.init = function (terrain)
    {
        if (terrain.getPalette() === "")
        {
            terrain.setPalette(SEA.getDefaultPalette());
        }
        terrain.setTerrainName(SEA.getName());
    };

    this.getName = function()
    {
        return qsTr("Sea");
    };
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        var surroundingsPlains = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Direct, false);
        var surroundingsBride = terrain.getSurroundings("BRIDGE1,BRIDGE2", false, false, GameEnums.Directions_North, false, true);
        if (surroundingsBride !== "" && terrain.existsResAnim("sea" + surroundingsPlains + "+bridge+mask"))
        {
            surroundingsPlains += "+bridge";
        }
        else
        {
            var surroundings = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_Direct, false);
            // load overlay north east
            if (!surroundings.includes("+N") && !surroundings.includes("+E"))
            {
                var surroundingsNE = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_NorthEast, false);
                if (surroundingsNE !== "")
                {
                    surroundingsPlains += surroundingsNE;
                }
            }
            // load overlay south east
            if (!surroundings.includes("+S") && !surroundings.includes("+E"))
            {
                var surroundingsSE = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_SouthEast, false);
                if (surroundingsSE !== "")
                {
                    surroundingsPlains += surroundingsSE;
                }
            }
            // load overlay south west
            if (!surroundings.includes("+S") && !surroundings.includes("+W"))
            {
                var surroundingsSW = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_SouthWest, false);
                if (surroundingsSW !== "")
                {
                    surroundingsPlains +=  surroundingsSW;
                }
            }
            // load overlay north west
            if (!surroundings.includes("+N") && !surroundings.includes("+W"))
            {
                var surroundingsNW = terrain.getSurroundings("SEA,LAKE", true, true, GameEnums.Directions_NorthWest, false);
                if (surroundingsNW !== "")
                {
                    surroundingsPlains += surroundingsNW;
                }
            }
        }
        terrain.loadBaseSprite("sea" + surroundingsPlains + "+mask");
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        var surroundingsSnow = terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow !== "")
        {
            terrain.loadOverlaySprite("sea+snow" + surroundingsSnow + "+mask", -1, -1, terrain.getNeighbourDirectionsPalette(surroundingsSnow, "SNOW"));
        }
        var surroundingsWaste = terrain.getSurroundings("WASTE", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsWaste !== "")
        {
            terrain.loadOverlaySprite("sea+waste" + surroundingsWaste + "+mask", -1, -1, terrain.getNeighbourDirectionsPalette(surroundingsWaste, "WASTE"));
        }
        var surroundingsDesert = terrain.getSurroundings("DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert !== "")
        {
            terrain.loadOverlaySprite("sea+desert" + surroundingsDesert + "+mask", -1, -1, terrain.getNeighbourDirectionsPalette(surroundingsDesert, "DESERT"));
        }
        var surroundingsRiver = terrain.getSurroundings("RIVER", true, false, GameEnums.Directions_Direct, false, false, 1, true);
        if (surroundingsRiver !== "")
        {
            RIVER.loadTerrainSeaOverlay(terrain, surroundingsRiver);
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_sea";
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return "back_" + weatherModifier +"sea";
    };
    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };
    this.getDescription = function()
    {
        return qsTr("Calm sea. Naval and air forces have good mobility.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["sea+mask",
                "sea+N+mask",
                "sea+E+mask",
                "sea+S+mask",
                "sea+W+mask",
                "sea+E+SW+NW+mask",
                "sea+E+SW+mask",
                "sea+E+NW+mask",
                "sea+N+SE+SW+mask",
                "sea+N+SE+mask",
                "sea+N+SW+mask",
                "sea+W+NE+SE+mask",
                "sea+W+NE+mask",
                "sea+W+SE+mask",
                "sea+S+NE+NW+mask",
                "sea+S+NE+mask",
                "sea+S+NW+mask",
                "sea+N+E+mask",
                "sea+E+S+mask",
                "sea+E+W+mask",
                "sea+N+S+mask",
                "sea+N+W+mask",
                "sea+S+W+mask",
                "sea+N+W+SE+mask",
                "sea+E+S+NW+mask",
                "sea+S+W+NE+mask",
                "sea+N+E+SW+mask",
                "sea+E+S+W+mask",
                "sea+N+E+S+mask",
                "sea+N+E+W+mask",
                "sea+N+S+W+mask",
                "sea+N+E+S+W+mask",
                "sea+NE+SE+SW+NW+mask",
                "sea+NE+SW+NW+mask",
                "sea+NE+SE+SW+mask",
                "sea+SE+SW+NW+mask",
                "sea+NE+SE+NW+mask",
                "sea+NE+NW+mask",
                "sea+NE+SE+mask",
                "sea+NE+SW+mask",
                "sea+SE+NW+mask",
                "sea+SW+NW+mask",
                "sea+SE+SW+mask",
                "sea+NE+mask",
                "sea+NW+mask",
                "sea+SE+mask",
                "sea+SW+mask",];
    };

    this.getOverlayTerrainSprites = function(map)
    {
        return ["sea+snow+E+mask",
                "sea+snow+E+S+mask",
                "sea+snow+E+S+W+mask",
                "sea+snow+E+W+mask",
                "sea+snow+N+mask",
                "sea+snow+N+E+mask",
                "sea+snow+N+E+S+mask",
                "sea+snow+N+E+S+W+mask",
                "sea+snow+N+E+W+mask",
                "sea+snow+N+S+mask",
                "sea+snow+N+S+W+mask",
                "sea+snow+N+W+mask",
                "sea+snow+S+mask",
                "sea+snow+S+W+mask",
                "sea+snow+W+mask",
                "sea+desert+E+mask",
                "sea+desert+E+S+mask",
                "sea+desert+E+S+W+mask",
                "sea+desert+E+W+mask",
                "sea+desert+N+mask",
                "sea+desert+N+E+mask",
                "sea+desert+N+E+S+mask",
                "sea+desert+N+E+S+W+mask",
                "sea+desert+N+E+W+mask",
                "sea+desert+N+S+mask",
                "sea+desert+N+S+W+mask",
                "sea+desert+N+W+mask",
                "sea+desert+S+mask",
                "sea+desert+S+W+mask",
                "sea+desert+W+mask",
                "sea+waste+E+mask",
                "sea+waste+E+S+mask",
                "sea+waste+E+S+W+mask",
                "sea+waste+E+W+mask",
                "sea+waste+N+mask",
                "sea+waste+N+E+mask",
                "sea+waste+N+E+S+mask",
                "sea+waste+N+E+S+W+mask",
                "sea+waste+N+E+W+mask",
                "sea+waste+N+S+mask",
                "sea+waste+N+S+W+mask",
                "sea+waste+N+W+mask",
                "sea+waste+S+mask",
                "sea+waste+S+W+mask",
                "sea+waste+W+mask",
                "sea+clear+E+mask",
                "sea+clear+E+S+mask",
                "sea+clear+E+S+W+mask",
                "sea+clear+E+W+mask",
                "sea+clear+N+mask",
                "sea+clear+N+E+mask",
                "sea+clear+N+E+S+mask",
                "sea+clear+N+E+S+W+mask",
                "sea+clear+N+E+W+mask",
                "sea+clear+N+S+mask",
                "sea+clear+N+S+W+mask",
                "sea+clear+N+W+mask",
                "sea+clear+S+mask",
                "sea+clear+S+W+mask",
                "sea+clear+W+mask",
                "beach+land+NE+overlay+mask",
                "beach+land+NW+overlay+mask",
                "beach+land+SE+overlay+mask",
                "beach+land+SW+overlay+mask",];
    };
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
};
Constructor.prototype = TERRAIN;
var SEA = new Constructor();
