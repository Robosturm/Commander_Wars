var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(WASTELAND.getName());
    };
    this.getName = function()
    {
        return qsTr("Wasteland");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 3);
        terrain.loadBaseSprite("wasteland+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_wasteland";
    };
    this.getDescription = function()
    {
        return qsTr("Infantry and Mech units aside, mobility is impaired on rough terrain.");
    };
};
Constructor.prototype = TERRAIN;
var WASTELAND = new Constructor();
