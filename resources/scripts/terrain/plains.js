var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Plains"));
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
        var surroundingsDesert = terrain.getSurroundings("SNOW_WASTELAND", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert.includes("+N"))
        {
            terrain.loadOverlaySprite("snow+N");
        }
        if (surroundingsDesert.includes("+E"))
        {
            terrain.loadOverlaySprite("snow+E");
        }
        if (surroundingsDesert.includes("+S"))
        {
            terrain.loadOverlaySprite("snow+S");
        }
        if (surroundingsDesert.includes("+W"))
        {
            terrain.loadOverlaySprite("snow+W");
        }
    };
};
Constructor.prototype = TERRAIN;
var PLAINS = new Constructor();
