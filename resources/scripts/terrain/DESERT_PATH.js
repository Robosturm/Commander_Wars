var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Path"));
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
        else
        {
            terrain.loadBaseTerrain("DESERT");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("STREET,BRIDGE,AIRPORT,FACTORY,HARBOUR,HQ,LABOR,MINE,SILO,SILO_ROCKET,TOWER,TOWN,RADAR,PIPESTATION,DESERT_PATH",
                                                   false, false, GameEnums.Directions_Direct, false, true);
        terrain.loadBaseSprite("desert_path" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_street";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_street";
    };
};
Constructor.prototype = TERRAIN;
var DESERT_PATH = new Constructor();
