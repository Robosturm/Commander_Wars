var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(LAKE.getName());
    };

    this.getName = function()
    {
        return qsTr("Lake");
    };
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        terrain.loadBaseSprite("lake");
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        var surroundings = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_Direct, false);
        if (surroundings !== "")
        {
            terrain.loadOverlaySprite("lake" + surroundings);
        }

        // load overlay south east
        if (!surroundings.includes("+S") && !surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("lake" + surroundingsSE);
            }
        }
        // load overlay north east
        if (!surroundings.includes("+N") && !surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("lake" + surroundingsNE);
            }
        }
        // load overlay south west
        if (!surroundings.includes("+S") && !surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("lake" + surroundingsSW);
            }
        }
        // load overlay north west
        if (!surroundings.includes("+N") && !surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("lake" + surroundingsNW);
            }
        }
        // load river overlay
        var surroundingLake = terrain.getSurroundings("LAKE,SEA", true, false, GameEnums.Directions_All, false);
        var surroundingsRiver = terrain.getSurroundings("RIVER", false, false, GameEnums.Directions_Direct, false);
        var surroundingsRiver2 = terrain.getSurroundings("RIVER", true, false, GameEnums.Directions_Direct, false, false, 1);
        // load overlay north
        if ((surroundingsRiver.includes("+N") || surroundingsRiver2.includes("+N")) &&
             surroundingLake.includes("+E") && surroundingLake.includes("+W"))
        {
            terrain.loadOverlaySprite("riverend_lake+N");
        }
        // load overlay east
        if ((surroundingsRiver.includes("+E") || surroundingsRiver2.includes("+E")) &&
             surroundingLake.includes("+S") && surroundingLake.includes("+N"))
        {
            terrain.loadOverlaySprite("riverend_lake+E");
        }
        // load overlay south
        if ((surroundingsRiver.includes("+S") || surroundingsRiver2.includes("+S")) &&
             surroundingLake.includes("+E") && surroundingLake.includes("+W"))
        {
            terrain.loadOverlaySprite("riverend_lake+S");
        }
        // load overlay west
        if ((surroundingsRiver.includes("+W") || surroundingsRiver2.includes("+W")) &&
             surroundingLake.includes("+S") && surroundingLake.includes("+N"))
        {
            terrain.loadOverlaySprite("riverend_lake+W");
        }
		
        var surroundingsSnow = terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow !== "")
        {
            terrain.loadOverlaySprite("lake+snow" + surroundingsSnow);
        }
        var surroundingsWaste = terrain.getSurroundings("WASTE", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsWaste !== "")
        {
            terrain.loadOverlaySprite("lake+waste" + surroundingsWaste);
        }
        var surroundingsDesert = terrain.getSurroundings("DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert !== "")
        {
            terrain.loadOverlaySprite("lake+desert" + surroundingsDesert);
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_sea";
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "fore_sea";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return "back_sea";
    };
    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };
    this.getDescription = function()
    {
        return qsTr("A calm inland body of water. Naval and air forces have good mobility while hovercraft can freely move on and off.");
    };
    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["lake"];
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return ["lake+NE",
                "lake+NW",
                "lake+SE",
                "lake+SW",
                "lake+E",
                "lake+E+S",
                "lake+E+S+W",
                "lake+E+W",
                "lake+N",
                "lake+N+E",
                "lake+N+E+S",
                "lake+N+E+S+W",
                "lake+N+E+W",
                "lake+N+S",
                "lake+N+S+W",
                "lake+N+W",
                "lake+S",
                "lake+S+W",
                "lake+W",
                "riverend_lake+E",
                "riverend_lake+S",
                "riverend_lake+W",
                "riverend_lake+N",
                "lake+snow+E+S",
                "lake+snow+E+S+W",
                "lake+snow+N+E",
                "lake+snow+N+E+S",
                "lake+snow+N+E+S+W",
                "lake+snow+N+E+W",
                "lake+snow+N+S+W",
                "lake+snow+N+W",
                "lake+snow+S+W",
                "lake+desert+E+S",
                "lake+desert+E+S+W",
                "lake+desert+N+E",
                "lake+desert+N+E+S",
                "lake+desert+N+E+S+W",
                "lake+desert+N+E+W",
                "lake+desert+N+S+W",
                "lake+desert+N+W",
                "lake+desert+S+W",
                "lake+waste+E+S",
                "lake+waste+E+S+W",
                "lake+waste+N+E",
                "lake+waste+N+E+S",
                "lake+waste+N+E+S+W",
                "lake+waste+N+E+W",
                "lake+waste+N+S+W",
                "lake+waste+N+W",
                "lake+waste+S+W",];
    };
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
};
Constructor.prototype = TERRAIN;
var LAKE = new Constructor();
