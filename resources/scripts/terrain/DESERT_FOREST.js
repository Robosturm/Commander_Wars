var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(DESERT_FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Desert Forest");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("DESERT");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("DESERT_FOREST", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("DESERT_FOREST", false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite("desert_forest" + surroundings);
    };
    this.loadOverlaySprite = function(terrain)
    {
        // Check every side.
        var surroundings = terrain.getSurroundings("DESERT_FOREST", false, false, GameEnums.Directions_Direct, false);
        // Load overlay south east, strict.
        if (surroundings.includes("+S") && surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("DESERT_FOREST", false, false, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("desert_forest+SE");
            }
        }
        // Load overlay north east, strict.
        if (surroundings.includes("+N") && surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("DESERT_FOREST", false, false, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("desert_forest+NE");
            }
        }
        // Load overlay south west, strict.
        if (surroundings.includes("+S") && surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("DESERT_FOREST", false, false, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("desert_forest+SW");
            }
        }
        // Load overlay northwest, strict.
        if (surroundings.includes("+N") && surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("DESERT_FOREST", false, false, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("desert_forest+NW");
            }
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert_forest";
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
    };
    this.getTerrainAnimationBase = function(unit, terrain)
    {
        return "base_desertforest";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 2);
        return "back_desertforest+" + rand.toString();
    };

    this.getDescription = function()
    {
        return "<r>" + qsTr("In Fog of War conditions, the woods provide ground unit hiding places.") + "</r>" +
                "<div c='#00ff00'>" + qsTr(" It reduces the firerange of indirect units by 1.") + "</div>";
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["desert_forest.png",
                "desert_forest+E",
                "desert_forest+E+W",
                "desert_forest+W"];
    };
    this.getTerrainAnimationBase = function(unit, terrain)
    {
        return "base_desertforest";
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender)
    {
        return TERRAIN.getFactoryForeground(terrain);
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 2);
        return "back_desertforest+" + rand.toString();
    };
};
Constructor.prototype = TERRAIN;
var DESERT_FOREST = new Constructor();
