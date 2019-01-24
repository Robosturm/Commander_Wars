var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Fire"));
    };
    this.getDefense = function()
    {
        return 0;
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("fire");
    };
};
Constructor.prototype = TERRAIN;
var FIRE = new Constructor();
