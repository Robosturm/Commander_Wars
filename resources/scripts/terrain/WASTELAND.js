var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Wasteland"));
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 3);
        terrain.loadBaseSprite("wasteland+" + random.toString());
    };
};
Constructor.prototype = TERRAIN;
var WASTELAND = new Constructor();
