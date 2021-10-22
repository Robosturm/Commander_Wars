var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(SNOW_FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Snowy Forest");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        terrain.loadBaseTerrain("SNOW");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("SNOW_FOREST", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("SNOW_FOREST", false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite("snow_forest" + surroundings);
    };
    this.loadOverlaySprite = function(terrain)
    {
        // Check every side.
        var surroundings = terrain.getSurroundings("SNOW_FOREST", false, false, GameEnums.Directions_Direct, false);
        // Load overlay south east, strict.
        if (surroundings.includes("+S") && surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("SNOW_FOREST", false, false, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("snow_forest+SE");
            }
        }
        // Load overlay north east, strict.
        if (surroundings.includes("+N") && surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("SNOW_FOREST", false, false, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("snow_forest+NE");
            }
        }
        // Load overlay south west, strict.
        if (surroundings.includes("+S") && surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("SNOW_FOREST", false, false, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("snow_forest+SW");
            }
        }
        // Load overlay northwest, strict.
        if (surroundings.includes("+N") && surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("SNOW_FOREST", false, false, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("snow_forest+NW");
            }
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow_forest";
    };
    this.getVisionHide = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War conditions, the snowy woods provide ground unit </r><div c='#00ff00'>hiding places.</div><r> It's hard for ground units to cross this terrain.</r>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["snow_forest.png",
                "snow_forest+E",
                "snow_forest+E+W",
                "snow_forest+W"];
    };
    this.getTerrainAnimationBase = function(unit, terrain)
    {
        return "base_snowforest";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 1);
        return "back_snowforest+" + rand.toString();
    };
};
Constructor.prototype = TERRAIN;
var SNOW_FOREST = new Constructor();
