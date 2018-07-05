var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.terrainName = qsTr("Plains");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 20);
        terrain.loadBaseSprite("plains+" + random.toString());
    };
};
Constructor.prototype = TERRAIN;
var PLAINS = new Constructor();
