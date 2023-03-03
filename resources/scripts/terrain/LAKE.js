var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setPalette("palette_clear");
        terrain.setTerrainName(LAKE.getName());
    };

    this.getName = function()
    {
        return qsTr("Lake");
    };
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        terrain.loadBaseSprite("lake+mask");
    };
    this.loadOverlaySprite = function(terrain, map)
    {        
        // load river overlay
        var surroundings = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_Direct, false);
        var surroundingsSea = terrain.getSurroundings("LAKE,SEA", true, false, GameEnums.Directions_Direct, false);
        var surroundingsRiver = terrain.getSurroundings("RIVER", false, false, GameEnums.Directions_Direct, false);
        var surroundingsRiver2 = terrain.getSurroundings("RIVER", true, false, GameEnums.Directions_Direct, false, false, 1);

        let placedSea = false;
        // load overlay north
        if ((surroundingsRiver.includes("+N") || surroundingsRiver2.includes("+N")))
        {
            if (terrain.existsResAnim("riverend+N_lake" + surroundingsSea + "+mask"))
            {
                terrain.loadOverlaySprite("riverend+N_lake" + surroundingsSea + "+mask");
                placedSea = true;
            }
        }
        // load overlay east
        if ((surroundingsRiver.includes("+E") || surroundingsRiver2.includes("+E")))
        {
            if (terrain.existsResAnim("riverend+E_lake" + surroundingsSea + "+mask"))
            {
                terrain.loadOverlaySprite("riverend+E_lake" + surroundingsSea + "+mask");
                placedSea = true;
            }
        }
        // load overlay south
        if ((surroundingsRiver.includes("+S") || surroundingsRiver2.includes("+S")))
        {
            if (terrain.existsResAnim("riverend+S_lake" + surroundingsSea + "+mask"))
            {
                terrain.loadOverlaySprite("riverend+S_lake" + surroundingsSea + "+mask");
                placedSea = true;
            }
        }
        // load overlay west
        if ((surroundingsRiver.includes("+W") || surroundingsRiver2.includes("+W")))
        {
            if (terrain.existsResAnim("riverend+W_lake" + surroundingsSea + "+mask"))
            {
                terrain.loadOverlaySprite("riverend+W_lake" + surroundingsSea + "+mask");
                placedSea = true;
            }
        }
        if (placedSea === false)
        {
            if (surroundings !== "")
            {
                terrain.loadOverlaySprite("lake" + surroundings + "+mask");
            }
            // load overlay south east
            if (!surroundings.includes("+S") && !surroundings.includes("+E"))
            {
                var surroundingsSE = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_SouthEast, false);
                if (surroundingsSE !== "")
                {
                    terrain.loadOverlaySprite("lake" + surroundingsSE + "+mask");
                }
            }
            // load overlay north east
            if (!surroundings.includes("+N") && !surroundings.includes("+E"))
            {
                var surroundingsNE = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_NorthEast, false);
                if (surroundingsNE !== "")
                {
                    terrain.loadOverlaySprite("lake" + surroundingsNE + "+mask");
                }
            }
            // load overlay south west
            if (!surroundings.includes("+S") && !surroundings.includes("+W"))
            {
                var surroundingsSW = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_SouthWest, false);
                if (surroundingsSW !== "")
                {
                    terrain.loadOverlaySprite("lake" + surroundingsSW + "+mask");
                }
            }
            // load overlay north west
            if (!surroundings.includes("+N") && !surroundings.includes("+W"))
            {
                var surroundingsNW = terrain.getSurroundings("LAKE,SEA", true, true, GameEnums.Directions_NorthWest, false);
                if (surroundingsNW !== "")
                {
                    terrain.loadOverlaySprite("lake" + surroundingsNW + "+mask");
                }
            }
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
        return ["lake+NE+mask",
                "lake+NW+mask",
                "lake+SE+mask",
                "lake+SW+mask",
                "lake+E+mask",
                "lake+E+S+mask",
                "lake+E+S+W+mask",
                "lake+E+W+mask",
                "lake+N+mask",
                "lake+N+E+mask",
                "lake+N+E+S+mask",
                "lake+N+E+S+W+mask",
                "lake+N+E+W+mask",
                "lake+N+S+mask",
                "lake+N+S+W+mask",
                "lake+N+W+mask",
                "lake+S+mask",
                "lake+S+W+mask",
                "lake+W+mask",
                "lake+snow+E+S+mask",
                "lake+snow+E+S+W+mask",
                "lake+snow+N+E+mask",
                "lake+snow+N+E+S+mask",
                "lake+snow+N+E+S+W+mask",
                "lake+snow+N+E+W+mask",
                "lake+snow+N+S+W+mask",
                "lake+snow+N+W+mask",
                "lake+snow+S+W+mask",
                "lake+desert+E+S+mask",
                "lake+desert+E+S+W+mask",
                "lake+desert+N+E+mask",
                "lake+desert+N+E+S+mask",
                "lake+desert+N+E+S+W+mask",
                "lake+desert+N+E+W+mask",
                "lake+desert+N+S+W+mask",
                "lake+desert+N+W+mask",
                "lake+desert+S+W+mask",
                "lake+waste+E+S+mask",
                "lake+waste+E+S+W+mask",
                "lake+waste+N+E+mask",
                "lake+waste+N+E+S+mask",
                "lake+waste+N+E+S+W+mask",
                "lake+waste+N+E+W+mask",
                "lake+waste+N+S+W+mask",
                "lake+waste+N+W+mask",
                "lake+waste+S+W+mask",

                "riverend+W_lake+mask",
                "riverend+W_lake+E+mask",
                "riverend+W_lake+N+mask",
                "riverend+W_lake+N+E+mask",
                "riverend+W_lake+N+E+S+mask",
                "riverend+W_lake+N+S+mask",
                "riverend+W_lake+E+S+mask",
                "riverend+W_lake+S+mask",
                "riverend+S_lake+mask",
                "riverend+S_lake+E+mask",
                "riverend+S_lake+E+W+mask",
                "riverend+S_lake+N+mask",
                "riverend+S_lake+N+E+mask",
                "riverend+S_lake+N+E+W+mask",
                "riverend+S_lake+N+W+mask",
                "riverend+S_lake+W+mask",
                "riverend+N_lake+mask",
                "riverend+N_lake+E+mask",
                "riverend+N_lake+E+S+mask",
                "riverend+N_lake+E+S+W+mask",
                "riverend+N_lake+E+W+mask",
                "riverend+N_lake+S+mask",
                "riverend+N_lake+S+W+mask",
                "riverend+N_lake+W+mask",
                "riverend+E_lake+mask",
                "riverend+E_lake+N+mask",
                "riverend+E_lake+N+S+mask",
                "riverend+E_lake+N+S+W+mask",
                "riverend+E_lake+N+W+mask",
                "riverend+E_lake+S+mask",
                "riverend+E_lake+S+W+mask",
                "riverend+E_lake+W+mask",];
    };
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
};
Constructor.prototype = TERRAIN;
var LAKE = new Constructor();
