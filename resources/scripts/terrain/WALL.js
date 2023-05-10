var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.getDefaultPalette = function()
    {
        return "palette_clear";
    };
    this.init = function (terrain)
    {
        terrain.setPalette(WALL.getDefaultPalette());
        terrain.setVisionHigh(2);
        terrain.setTerrainName(WALL.getName());
    };
    this.getName = function()
    {
        return qsTr("Wall");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID, map, currentPalette)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW", currentPalette);
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT", currentPalette);
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE", currentPalette);
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS", currentPalette);
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS", currentPalette);
        }
    };
    this.loadBaseSprite = function(terrain, map)
    {
        var surroundings = terrain.getSurroundings("WALL,ZGATE_E_W,ZGATE_N_S,WEAK_WALL,ZDESTROYED_GATE_E_W,ZDESTROYED_GATE_N_S", false, false, GameEnums.Directions_Direct, true, true);
        terrain.loadBaseSprite("wall" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_pipeline";
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "fore_pipeline";
    };
    this.getDescription = function()
    {
        return qsTr("Dark Matter Walls can't be crossed by most units. They can be destroyed at gates and weak wall parts.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["wall",
                "wall+E",
                "wall+E+S",
                "wall+E+S+W",
                "wall+E+W",
                "wall+N",
                "wall+N+E",
                "wall+N+E+S",
                "wall+N+E+S+W",
                "wall+N+E+W",
                "wall+N+S",
                "wall+N+S+W",
                "wall+N+W",
                "wall+S",
                "wall+S+W",
                "wall+W"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "fore_walltop";
    };
    this.getEditorPlacementSound = function()
    {
        return "placeBuilding.wav";
    };
};
Constructor.prototype = TERRAIN;
var WALL = new Constructor();
