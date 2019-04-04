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

    // vision created by this field
    getVision : function()
    {
        return -1;
    },

    getBonusVision : function(unit)
    {
        return 0;
    },

    getVisionHide : function()
    {
        return false;
    },

    getMiniMapIcon : function()
    {
        return "minimap_plains";
    },

    onDestroyed : function(terrain)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
    },

    getTerrainAnimationBase : function(unit, terrain)
    {
        return "base_air";
    },

    getTerrainAnimationForeground : function(unit, terrain)
    {
        var rand = globals.randInt(0, 2);
        if (rand === 2)
        {
            return "fore_plains";
        }
        else
        {
            return "fore_plains+" + rand.toString();
        }
    },

    getTerrainAnimationBackground : function(unit, terrain)
    {

        var id = "PLAINS";

        var y = terrain.getY() - 1;
        var x = terrain.getX();
        if (map.onMap(x, y))
        {
            var upTerrain = map.getTerrain(x, y);
            id = upTerrain.getID();
            if (upTerrain.getBuilding() !== null)
            {
                id = "BUILDING";
            }
        }
        switch (id)
        {
            case "BRIDGE":
                return "back_bridge";
            case "SEA":
            case "BEACH":
            case "FOG":
            case "REAF":
            case "ROUGH_SEA":
                return "back_cliff";
            case "FOREST":
                return "back_forest+0";
            case "MOUNTAIN":
                return "back_mountain+0";
            case "BUILDING":
                return "back_town+0";
            case "STREET":
                return "back_street";
            default:
                return "back_plains";
        }
    },
};
