// this is the base class for terrain
var TERRAIN =
{
    // search type filter for overlay
    All : 0,
    Direct : 1,
    Diagnonal : 2,

    // loader for stuff which needs C++ Support
    init : function (terrain)
    {
        terrain.Animated = false;
    },
    getDefense : function()
    {
        return 0;
    },
    loadBaseTerrain : function(terrain)
    {
    },
    loadBaseSprite : function(terrain)
    {
    },
    loadOverlaySprite : function(terrain)
    {
    },
};
