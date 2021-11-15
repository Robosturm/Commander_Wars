var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Forest");
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
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };

    this.loadBase = function(terrain, terrainId, spriteId)
    {
        var surroundings = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite(spriteId + surroundings);
    };
    this.loadOverlay = function(terrain, terrainId, spriteId)
    {
        // Check every side.
        var surroundings = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_Direct, false);
        // Load overlay south east, strict.
        if (surroundings.includes("+S") && surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("FOREST", false, false, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite(spriteId + "+SE");
            }
        }
        // Load overlay north east, strict.
        if (surroundings.includes("+N") && surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite(spriteId + "+NE");
            }
        }
        // Load overlay south west, strict.
        if (surroundings.includes("+S") && surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite(spriteId + "+SW");
            }
        }
        // Load overlay northwest, strict.
        if (surroundings.includes("+N") && surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite(spriteId + "+NW");
            }
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_forest";
    };
    this.getVisionHide = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War conditions, the woods provide ground unit</r><div c='#00ff00'>hiding places.</div>");
    };
    this.getSprites = function(spriteId)
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [spriteId,
                spriteId + "+E",
                spriteId + "+E+W",
                spriteId + "+W"];
    };

    this.getTerrainAnimationBase = function(unit, terrain)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "base_" + weatherModifier + "forest";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 2);
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "back_" + weatherModifier + "forest+" + rand.toString();
    };
};
Constructor.prototype = TERRAIN;
var __BASEFOREST = new Constructor();
