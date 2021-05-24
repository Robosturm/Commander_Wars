// this is the base class for terrain
var TERRAIN =
{
    // search type filter for overlay
    getTerrainGroup : function()
    {
        // terrain groups for editor selection ordering
        // 0 = sea tiles
        // 1 = normal ground tiles
        // 2 = desert tiles
        // 3 = snow tiles
        // everthing else is open to use
        return 0;
    },
    getTerrainGroupSort : function()
    {
        return [1,
                0,
                2,
                3,];
    },
    terrainGroupNameMapping = [qsTr("Sea"),
                               qsTr("Normal"),
                               qsTr("Desert"),
                               qsTr("Snow")],

    getTerrainGroupName : function(item)
    {
        if (item >= 0 && item < terrainGroupNameMapping.length)
        {
            return terrainGroupNameMapping[item];
        }
        return qsTr("Unknown");
    },

    // loader for stuff which needs C++ Support
    init : function (terrain)
    {
    },
    // returns the defense of this terrain
    getDefense : function(terrain)
    {
        return 0;
    },

    getFirerangeModifier : function(terrain, unit)
    {
        return 0;
    },

    getMinFirerangeModifier : function(terrain, unit)
    {
        return 0;
    },

    // called for loading a sub image terrain
    // this has only a visual effect
    loadBaseTerrain : function(terrain, currentTerrainID)
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

    getName : function()
    {
        return "";
    },

    // vision created by this field
    getVision : function(player, terrain)
    {
        return -1;
    },

    getBonusVision : function(unit, terrain)
    {
        return 0;
    },

    getVisionHide : function(terrain, player)
    {
        return false;
    },

    getMiniMapIcon : function(terrain)
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

    getTerrainAnimationMoveSpeed : function(terrain)
    {
        return 0;
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

    getDescription : function(terrain)
    {
        return "";
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

    getTerrainSprites : function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [];
    },

    startOfTurn : function(terrain)
    {
        // needs to be enable by calling setHasStartOfTurn(true) in the js function init
    },

    // additional offensive bonus for a unit on this field
    getOffensiveFieldBonus : function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
        return 0;
    },
    //  additional deffensive bonus for a unit on this field
    getDeffensiveFieldBonus : function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender, action)
    {
        return 0;
    },
};
