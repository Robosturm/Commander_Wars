var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Street"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("STREET,BRIDGE", false, false, GameEnums.Directions_Direct, false);
        terrain.loadBaseSprite("street" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_street";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_street";
    };
};
Constructor.prototype = TERRAIN;
var STREET = new Constructor();
