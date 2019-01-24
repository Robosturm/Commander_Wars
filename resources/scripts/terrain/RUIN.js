var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Ruin"));
    };

    this.getDefense = function()
    {
        return 1;
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 2);
        terrain.loadBaseSprite("ruin+" + random.toString());
    };
};
Constructor.prototype = TERRAIN;
var RUIN = new Constructor();
