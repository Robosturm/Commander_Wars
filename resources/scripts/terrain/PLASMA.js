var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Plasma"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PLASMA,METEOR", false, false, GameEnums.Directions_Direct);
        terrain.loadBaseSprite("plasma" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plasma";
    };
};
Constructor.prototype = TERRAIN;
var PLASMA = new Constructor();
