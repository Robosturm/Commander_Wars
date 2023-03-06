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
        terrain.setPalette(PLAINS_DESTROYED.getDefaultPalette());
        terrain.setTerrainName(PLAINS_DESTROYED.getName());
    };
    this.getName = function()
    {
        return qsTr("Plains");
    };
    this.getDefense = function()
    {
        return 1;
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
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain, map)
    {
        terrain.loadBaseSprite("plains+destroyed");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plains";
    };
    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain.");
    };
};
Constructor.prototype = TERRAIN;
var PLAINS_DESTROYED = new Constructor();
