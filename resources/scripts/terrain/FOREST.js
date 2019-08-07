var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Forest");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 1);
        terrain.loadBaseSprite("forest+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_forest";
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
        return qsTr("In Fog of War conditions, the woods provide ground unit <div c='00ff00'>hiding places.</div>");
    };
};
Constructor.prototype = TERRAIN;
var FOREST = new Constructor();
