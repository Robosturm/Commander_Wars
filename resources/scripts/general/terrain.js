// this is the base class for terrain
var TERRAIN =
{
    // loader for stuff which needs C++ Support
    init : function (terrain)
    {
        terrain.Animated = false;
    },
    getDefense : function()
    {
        return 0;
    },
    loadBaseTerrainSprite : function(terrain)
    {
    },
    loadTerrainSprite : function(terrain)
    {
    },
    loadTerrainOverlaySprite : function(terrain)
    {
    },
};
