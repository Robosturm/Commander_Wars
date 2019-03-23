var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Meteor"));
        terrain.setHp(100);
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PLASMA", false, false, GameEnums.Directions_Direct, false);
		// get rid of the north identifier
		surroundings = surroundings.replace("+N", "");
        terrain.loadBaseSprite("meteor" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_meteor";
    };
};
Constructor.prototype = TERRAIN;
var METEOR = new Constructor();
