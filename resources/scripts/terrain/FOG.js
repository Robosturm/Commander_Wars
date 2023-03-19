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
            terrain.setPalette(FOG.getDefaultPalette());
        }
        terrain.setVisionHigh(1);
        terrain.setTerrainName(FOG.getName());
    };
    this.getName = function()
    {
        return qsTr("Fog");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID, map, currentPalette)
    {
        if (currentTerrainID === "LAKE")
        {
            terrain.loadBaseTerrain("LAKE", currentPalette);
        }
        else if (currentTerrainID === "SEA")
        {
            terrain.loadBaseTerrain("SEA", currentPalette);
        }
        else
        {
            terrain.loadBaseTerrain("SEA");
        }
    };
    this.loadBaseSprite = function(terrain, map)
    {
        var surroundings = terrain.getSurroundings("FOG", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("FOG", false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite("fog" + surroundings + "+mask");
    };
    this.loadOverlaySprite = function(terrain, map)
    {
		// Check every side.
        var surroundings = terrain.getSurroundings("FOG", false, false, GameEnums.Directions_Direct, false);
        // Load overlay south east, strict.
        if (surroundings.includes("+S") && surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("FOG", false, false, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("fog+SE+mask");
            }
        }
        // Load overlay north east, strict.
        if (surroundings.includes("+N") && surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("FOG", false, false, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("fog+NE+mask");
            }
        }
        // Load overlay south west, strict.
        if (surroundings.includes("+S") && surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("FOG", false, false, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("fog+SW+mask");
            }
        }
        // Load overlay northwest, strict.
        if (surroundings.includes("+N") && surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("FOG", false, false, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("fog+NW+mask");
            }
        }
	};
    this.getMiniMapIcon = function()
    {
        return "minimap_fog";
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
        return qsTr("<r>This heavy fog provides ideal hiding places</r><div c='#00ff00'> for naval units in Fog of War.</div>");
    };

    this.getTerrainSprites = function()
    {
        return ["fog.png",
                "fog+E",
                "fog+E+W",
                "fog+W"];
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return ["fog+SE",
                "fog+NE",
                "fog+SW",
                "fog+NW",];
    };
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
    this.useTerrainAsBaseTerrain = function()
    {
        return true;
    };
};
Constructor.prototype = TERRAIN;
var FOG = new Constructor();
