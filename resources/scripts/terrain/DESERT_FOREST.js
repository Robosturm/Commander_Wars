var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Desert Forest"));
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("DESERT");
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("desert_forest");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert_forest";
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
    };
    this.getTerrainAnimationBase = function(unit, terrain)
    {
        return "base_forest";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_forest";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_forest";
    };
};
Constructor.prototype = TERRAIN;
var DESERT_FOREST = new Constructor();
