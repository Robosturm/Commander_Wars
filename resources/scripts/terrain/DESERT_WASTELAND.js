var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESERT_WASTELAND.getName());
    };
    this.getName = function()
    {
        return qsTr("Desert Wasteland");
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
    this.getDescription = function()
    {
        return qsTr("Infantry and Mech units aside, mobility is impaired on rough terrain. It reduces the firerange of indirect units by 1.");
    };
};
Constructor.prototype = TERRAIN;
var DESERT_WASTELAND = new Constructor();
