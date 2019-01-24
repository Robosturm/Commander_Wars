var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Plains"));
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 140);
        if (random >= 15)
        {
            random = globals.randInt(15, 20);
            terrain.loadBaseSprite("plains+" + random.toString());
        }
        else
        {
            terrain.loadBaseSprite("plains+" + random.toString());
        }
    };
};
Constructor.prototype = TERRAIN;
var PLAINS = new Constructor();
