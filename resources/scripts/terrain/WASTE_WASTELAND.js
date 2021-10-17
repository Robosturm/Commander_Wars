var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(WASTE_WASTELAND.getName());
    };
    this.getName = function()
    {
        return qsTr("Waste Wasteland");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        terrain.loadBaseTerrain("WASTE");
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 3);
        terrain.loadBaseSprite("waste_wasteland+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_waste_wasteland";
    };
    this.getDescription = function()
    {
        return qsTr("<r>Waste terrain with reduced defense but clear view. In Fog of War, unit's gain </r><div c='#00ff00'>vision +1.</div><r> Mobility is impaired on this rough terrain.</r>");
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_wastewasteland";
    };
};
Constructor.prototype = TERRAIN;
var WASTE_WASTELAND = new Constructor();
