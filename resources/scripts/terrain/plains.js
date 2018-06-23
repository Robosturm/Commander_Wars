var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        // default a terrain has no animation
        terrain.Animated = false;
    };
    this.loadBaseTerrainSprite = function(terrain)
    {
        // tell the engine which sprite we wanna use
        terrain.loadBaseTerrainSprite("plains");
    };
};
Constructor.prototype = TERRAIN;
var PLAINS = new Constructor();
