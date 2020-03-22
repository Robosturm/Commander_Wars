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
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("snow_wasteland");
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
        return "fore_snow+0";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_snow";
    };
};
Constructor.prototype = TERRAIN;
var SNOW_WASTELAND = new Constructor();
