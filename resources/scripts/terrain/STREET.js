var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(STREET.getName());
    };
    this.getName = function()
    {
        return qsTr("Street");
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
        var surroundings = terrain.getSurroundings("STREET,BRIDGE,AIRPORT,FACTORY,HARBOUR,HQ,LABOR,MINE,SILO,SILO_ROCKET,TOWER,TOWN,RADAR,PIPESTATION,DESERT_PATH",
                                                   false, false, GameEnums.Directions_Direct, false, true);
        terrain.loadBaseSprite("street" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_street";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_street";
    };
    this.getDescription = function()
    {
        return qsTr("Well-surfaced roads provides optimum mobility but little cover.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["street",
                "street+E",
                "street+E+S",
                "street+E+S+W",
                "street+E+W",
                "street+N",
                "street+N+E",
                "street+N+E+S",
                "street+N+E+S+W",
                "street+N+E+W",
                "street+N+S",
                "street+N+S+W",
                "street+N+W",
                "street+S",
                "street+S+W",
                "street+W"];
    };
};
Constructor.prototype = TERRAIN;
var STREET = new Constructor();
