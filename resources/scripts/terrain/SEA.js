var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
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
        if (surroundingsBride !== "" && terrain.existsResAnim("sea" + surroundingsPlains + "+bridge"))
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
        terrain.loadBaseSprite("sea" + surroundingsPlains);
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        var surroundingsSnow = terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow !== "")
        {
            terrain.loadOverlaySprite("sea+snow" + surroundingsSnow);
        }
        var surroundingsWaste = terrain.getSurroundings("WASTE", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsWaste !== "")
        {
            terrain.loadOverlaySprite("sea+waste" + surroundingsWaste);
        }
        var surroundingsDesert = terrain.getSurroundings("DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert !== "")
        {
            terrain.loadOverlaySprite("sea+desert" + surroundingsDesert);
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
        return ["sea",
                "sea+N",
                "sea+E",
                "sea+S",
                "sea+W",
                "sea+E+SW+NW",
                "sea+E+SW",
                "sea+E+NW",
                "sea+N+SE+SW",
                "sea+N+SE",
                "sea+N+SW",
                "sea+W+NE+SE",
                "sea+W+NE",
                "sea+W+SE",
                "sea+S+NE+NW",
                "sea+S+NE",
                "sea+S+NW",
                "sea+N+E",
                "sea+E+S",
                "sea+E+W",
                "sea+N+S",
                "sea+N+W",
                "sea+S+W",
                "sea+N+W+SE",
                "sea+E+S+NW",
                "sea+S+W+NE",
                "sea+N+E+SW",
                "sea+E+S+W",
                "sea+N+E+S",
                "sea+N+E+W",
                "sea+N+S+W",
                "sea+N+E+S+W",
                "sea+NE+SE+SW+NW",
                "sea+NE+SW+NW",
                "sea+NE+SE+SW",
                "sea+SE+SW+NW",
                "sea+NE+SE+NW",
                "sea+NE+NW",
                "sea+NE+SE",
                "sea+NE+SW",
                "sea+SE+NW",
                "sea+SW+NW",
                "sea+SE+SW",
                "sea+NE",
                "sea+NW",
                "sea+SE",
                "sea+SW",];
    };

    this.getOverlayTerrainSprites = function(map)
    {
        return ["sea+snow+E",
                "sea+snow+E+S",
                "sea+snow+E+S+W",
                "sea+snow+E+W",
                "sea+snow+N",
                "sea+snow+N+E",
                "sea+snow+N+E+S",
                "sea+snow+N+E+S+W",
                "sea+snow+N+E+W",
                "sea+snow+N+S",
                "sea+snow+N+S+W",
                "sea+snow+N+W",
                "sea+snow+S",
                "sea+snow+S+W",
                "sea+snow+W",
                "sea+desert+E",
                "sea+desert+E+S",
                "sea+desert+E+S+W",
                "sea+desert+E+W",
                "sea+desert+N",
                "sea+desert+N+E",
                "sea+desert+N+E+S",
                "sea+desert+N+E+S+W",
                "sea+desert+N+E+W",
                "sea+desert+N+S",
                "sea+desert+N+S+W",
                "sea+desert+N+W",
                "sea+desert+S",
                "sea+desert+S+W",
                "sea+desert+W",
                "sea+waste+E",
                "sea+waste+E+S",
                "sea+waste+E+S+W",
                "sea+waste+E+W",
                "sea+waste+N",
                "sea+waste+N+E",
                "sea+waste+N+E+S",
                "sea+waste+N+E+S+W",
                "sea+waste+N+E+W",
                "sea+waste+N+S",
                "sea+waste+N+S+W",
                "sea+waste+N+W",
                "sea+waste+S",
                "sea+waste+S+W",
                "sea+waste+W",];
    };
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
};
Constructor.prototype = TERRAIN;
var SEA = new Constructor();
