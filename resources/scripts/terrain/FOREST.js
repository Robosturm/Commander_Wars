var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Forest"));
    };
    this.getDefense = function()
    {
        return 2;
    };
	this.loadBaseTerrain = function(terrain)
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
};
Constructor.prototype = TERRAIN;
var FOREST = new Constructor();
