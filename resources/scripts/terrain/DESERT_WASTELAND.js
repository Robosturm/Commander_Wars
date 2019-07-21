var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Desert Wasteland"));
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("desert_wasteland");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert_wasteland";
    };
};
Constructor.prototype = TERRAIN;
var DESERT_WASTELAND = new Constructor();
