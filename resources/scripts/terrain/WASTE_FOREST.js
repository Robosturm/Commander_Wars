var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(WASTE_FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Waste Forest");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else
        {
            terrain.loadBaseTerrain("WASTE");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("WASTE_FOREST", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("WASTE_FOREST", false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite("waste_forest" + surroundings);
    };
    this.loadOverlaySprite = function(terrain)
    {
        // Check every side.
        var surroundings = terrain.getSurroundings("WASTE_FOREST", false, false, GameEnums.Directions_Direct, false);
        // Load overlay south east, strict.
        if (surroundings.includes("+S") && surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("WASTE_FOREST", false, false, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("waste_forest+SE");
            }
        }
        // Load overlay north east, strict.
        if (surroundings.includes("+N") && surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("WASTE_FOREST", false, false, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("waste_forest+NE");
            }
        }
        // Load overlay south west, strict.
        if (surroundings.includes("+S") && surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("WASTE_FOREST", false, false, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("waste_forest+SW");
            }
        }
        // Load overlay northwest, strict.
        if (surroundings.includes("+N") && surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("WASTE_FOREST", false, false, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("waste_forest+NW");
            }
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_waste_forest";
    };
    this.getVisionHide = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War conditions, the snowy woods provide ground unit </r><div c='#00ff00'>hiding places.</div><r> Waste terrain with reduced defense but clear view. In Fog of War, unit's gain </r><div c='#00ff00'>vision +1.</div>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["waste_forest.png",
                "waste_forest+E",
                "waste_forest+E+W",
                "waste_forest+W"];
    };
    this.getTerrainAnimationBase = function(unit, terrain)
    {
        return "base_wasteforest";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_wasteforest";
    };
};
Constructor.prototype = TERRAIN;
var WASTE_FOREST = new Constructor();
