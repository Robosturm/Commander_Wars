// this is the base class for terrain
var TERRAIN =
{
    // search type filter for overlay

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
    // only bridges use this function for now.
    // all other replace the terrain complete with themself
    // and their base terrain
    useTerrainAsBaseTerrain : function()
    {
        return false;
    },

};
