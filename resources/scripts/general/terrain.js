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
    // returns the defense of this terrain
    getDefense : function()
    {
        return 0;
    },
    // called for loading a sub image terrain
    // this has only a visual effect
    loadBaseTerrain : function(terrain)
    {
    },
    // called for loading the main sprite
    loadBaseSprite : function(terrain)
    {
    },
    // called for loading an overlay sprite
    loadOverlaySprite : function(terrain)
    {
    },


    /***********************************************************************************/
    // this section deals with map editor hints
    // these functions have no influence in the game.
    /***********************************************************************************/
    // defines if a terrain can be placed in the editor at a given location
    canBePlaced : function(x, y)
    {
        return true;
    },
    placeTerrain : function(x, y)
    {

    },
};
