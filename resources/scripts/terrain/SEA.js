var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        // default a terrain has no animation
        terrain.Animated = true;
    };

    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("sea");
    };
};
Constructor.prototype = TERRAIN;
var SEA = new Constructor();
