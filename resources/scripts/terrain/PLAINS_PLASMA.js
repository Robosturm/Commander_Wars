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
        terrain.setPalette(PLAINS_PLASMA.getDefaultPalette());
        terrain.setTerrainName(PLAINS_PLASMA.getName());
    };
    this.getName = function()
    {
        return qsTr("Plains");
    };
	this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID, map)
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
            terrain.loadBaseTerrain("DESERT");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        var surroundings = terrain.getSurroundings("PLAINS_PLASMA", false, false, GameEnums.Directions_Direct);
        terrain.loadBaseSprite("plains+plasma" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plains";
    };
    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["plains+plasma",
                "plains+plasma+E",
                "plains+plasma+E+S",
                "plains+plasma+E+S+W",
                "plains+plasma+E+W",
                "plains+plasma+N",
                "plains+plasma+N+E",
                "plains+plasma+N+E+S",
                "plains+plasma+N+E+S+W",
                "plains+plasma+N+E+W",
                "plains+plasma+N+S",
                "plains+plasma+N+S+W",
                "plains+plasma+N+W",
                "plains+plasma+S",
                "plains+plasma+S+W",
                "plains+plasma+W"];
    };
};
Constructor.prototype = TERRAIN;
var PLAINS_PLASMA = new Constructor();
