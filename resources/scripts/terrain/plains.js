var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(PLAINS.getName());
    };
    this.getName = function()
    {
        return qsTr("Plains");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain, currentTerrainID)
    {
        var random = globals.randInt(0, 140);
        if (random >= 15)
        {
            random = globals.randInt(15, 20);
            terrain.loadBaseSprite("plains+" + random.toString());
        }
        else
        {
            terrain.loadBaseSprite("plains+" + random.toString());
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plains";
    };
    this.loadOverlaySprite = function(terrain)
    {
        var surroundingsSnow = terrain.getSurroundings("SNOW_WASTELAND", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow.includes("+N"))
        {
            terrain.loadOverlaySprite("snow+N");
        }
        if (surroundingsSnow.includes("+E"))
        {
            terrain.loadOverlaySprite("snow+E");
        }
        if (surroundingsSnow.includes("+S"))
        {
            terrain.loadOverlaySprite("snow+S");
        }
        if (surroundingsSnow.includes("+W"))
        {
            terrain.loadOverlaySprite("snow+W");
        }
        var surroundingsDesert = terrain.getSurroundings("DESERT_WASTELAND", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert.includes("+N"))
        {
            terrain.loadOverlaySprite("desert+N");
        }
        if (surroundingsDesert.includes("+E"))
        {
            terrain.loadOverlaySprite("desert+E");
        }
        if (surroundingsDesert.includes("+S"))
        {
            terrain.loadOverlaySprite("desert+S");
        }
        if (surroundingsDesert.includes("+W"))
        {
            terrain.loadOverlaySprite("desert+W");
        }
    };
    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["plains+0",
                "plains+1",
                "plains+2",
                "plains+3",
                "plains+4",
                "plains+5",
                "plains+6",
                "plains+7",
                "plains+8",
                "plains+9",
                "plains+10",
                "plains+11",
                "plains+12",
                "plains+13",
                "plains+14",
                "plains+15",
                "plains+16",
                "plains+17",
                "plains+18",
                "plains+19",
                "plains+20"];
    };
};
Constructor.prototype = TERRAIN;
var PLAINS = new Constructor();
