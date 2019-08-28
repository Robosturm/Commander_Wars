var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
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
        if (surroundings === "")
        {
            terrain.loadBaseSprite("wall+E+W");
        }
        else
        {
            terrain.loadBaseSprite("wall" + surroundings);
        }
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
};
Constructor.prototype = TERRAIN;
var WALL = new Constructor();
