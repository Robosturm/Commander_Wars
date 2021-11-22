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
        terrain.setTerrainName(PLASMA.getName());
    };
    this.getName = function()
    {
        return qsTr("Plasma");
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
        else if (currentTerrainID === "LAKE")
        {
            terrain.loadBaseTerrain("LAKE");
        }
        else if (currentTerrainID === "SEA")
        {
            terrain.loadBaseTerrain("SEA");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PLASMA,METEOR", false, false, GameEnums.Directions_Direct);
        terrain.loadBaseSprite("plasma" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plasma";
    };
    this.getDescription = function()
    {
        return qsTr("<r>Electric charge us impassable but disappears if </r><div c='#00ff00'> meteor pieces</div><r> are destroyed.</r>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["plasma",
                "plasma+E",
                "plasma+E+S",
                "plasma+E+S+W",
                "plasma+E+W",
                "plasma+N",
                "plasma+N+E",
                "plasma+N+E+S",
                "plasma+N+E+S+W",
                "plasma+N+E+W",
                "plasma+N+S",
                "plasma+N+S+W",
                "plasma+N+W",
                "plasma+S",
                "plasma+S+W",
                "plasma+W"];
    };
};
Constructor.prototype = TERRAIN;
var PLASMA = new Constructor();
