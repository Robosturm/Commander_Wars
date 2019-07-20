var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Snowy Forest"));
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("SNOW");
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("snow_forest");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow_forest";
    };
    this.getVisionHide = function()
    {
        return true;
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
var SNOW_FOREST = new Constructor();
