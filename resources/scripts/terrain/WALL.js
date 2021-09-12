var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(2);
        terrain.setTerrainName(WALL.getName());
    };
    this.getName = function()
    {
        return qsTr("Wall");
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
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("WALL,ZGATE_E_W,ZGATE_N_S,WEAK_WALL", false, false, GameEnums.Directions_Direct, true, true);
        terrain.loadBaseSprite("wall" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_pipeline";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
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
        return ["wall+E",
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
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_walltop";
    };
    this.getDefense = function()
    {
        return 4;
    };
};
Constructor.prototype = TERRAIN;
var WALL = new Constructor();
