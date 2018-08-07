// this is the base class for terrain
var BUILDING =
{

    // loader for stuff which needs C++ Support
    init : function (building)
    {
    },
    // returns the defense of this terrain
    getDefense : function()
    {
        return 3;
    },

    // called for loading the main sprites
    loadSprites : function(building)
    {
    },
    // the terrain on which a building can be placed
    // if tzhe current terrain isn't in the list. it'll be replaced by the first :)
    getBaseTerrain : function(building)
    {
        return "PLAINS,STREET"
    },

    canBuildingBePlaced : function(terrainID)
    {
        if ((terrainID === "PLAINS") || (terrainID === "STREET"))
        {
            return true;
        }
        else
        {
            return false;
        }
    },
};
