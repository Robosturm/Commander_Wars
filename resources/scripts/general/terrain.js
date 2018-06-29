// this is the base class for terrain
var TERRAIN =
{
    // search type filter for overlay

    North       : 0,
    NorthEast   : 1,
    East        : 2,
    SouthEast   : 3,
    South       : 4,
    SouthWest   : 5,
    West        : 6,
    NorthWest   : 7,
    All         : 8,
    Direct      : 9,
    Diagnonal   : 10,

    // loader for stuff which needs C++ Support
    init : function (terrain)
    {
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
