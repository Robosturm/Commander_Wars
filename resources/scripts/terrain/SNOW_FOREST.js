var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(SNOW_FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Snowy Forest");
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
    this.getDescription = function()
    {
        return qsTr("In Fog of War conditions, the snowy woods provide ground unit <div c='00ff00'>hiding places.</div> It's hard for ground units to cross this terrain.");
    };
};
Constructor.prototype = TERRAIN;
var SNOW_FOREST = new Constructor();
