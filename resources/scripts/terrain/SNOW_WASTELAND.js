var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Snowy Wasteland"));
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("snow_wasteland");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow_wasteland";
    };
};
Constructor.prototype = TERRAIN;
var SNOW_WASTELAND = new Constructor();
