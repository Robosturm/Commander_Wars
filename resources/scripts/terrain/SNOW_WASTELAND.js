var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(SNOW_WASTELAND.getName());
    };
    this.getName = function()
    {
        return qsTr("Snowy Wasteland");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
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
            terrain.loadBaseTerrain("SNOW");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 5);
        terrain.loadBaseSprite("snow_wasteland+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow_wasteland";
    };
    this.getDescription = function()
    {
        return qsTr("Mobility is impaired on this rough terrain.");
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_snowwasteland";
    };
};
Constructor.prototype = TERRAIN;
var SNOW_WASTELAND = new Constructor();
