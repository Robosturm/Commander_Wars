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
    this.loadBaseTerrain = function(terrain)
    {
        terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("plains+destroyed");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plains";
    };
};
Constructor.prototype = TERRAIN;
var PLAINS_DESTROYED = new Constructor();
