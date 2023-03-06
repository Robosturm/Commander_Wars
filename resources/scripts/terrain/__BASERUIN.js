var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setPalette(Global[terrain.getTerrainID()].getDefaultPalette());
        terrain.setVisionHigh(1);
        terrain.setTerrainName(Global[terrain.getTerrainID()].getName(terrain));
    };
    this.baseTerrainId = "PLAINS";
    this.getName = function(terrain = null)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return qsTr("Waste Ruin");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Snowy Ruin");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Desert Ruin");
        }
        else
        {
            return qsTr("Ruin");
        }
    };
    this.getDefense = function(terrain)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return 0;
        }
        else
        {
            return 1;
        }
    };
    this.getBonusVision = function(unit, terrain)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return 1;
        }
        return 0;
    };
    this.getOffensiveFieldBonus = function(terrain, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender, action, luckMode)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "DESERT")
        {
            return -20;
        }
        return 0;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID, map)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else
        {
            var baseTerrainId = ""
            if (terrain !== null)
            {
                baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
            }
            terrain.loadBaseTerrain(baseTerrainId);
        }
    };

    this.loadBase = function(terrain, spriteId, map)
    {
        var random = globals.randInt(0, 2);
        terrain.loadBaseSprite(spriteId + "+" + random.toString());
    };
    this.getMiniMapIcon = function(terrain)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return "minimap_waste_ruin";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "minimap_snow_ruin";
        }
        else if (baseTerrainId === "DESERT")
        {
            return "minimap_desert_ruin";
        }
        else
        {
            return "minimap_ruin";
        }
    };
    this.getVisionHide = function()
    {
        return true;
    };

    this.getDescription = function(terrain)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return qsTr("<r>In Fog of War conditions, the ruins provide ground unit </r><div c='#00ff00'>hiding places.</div><r> Snowy terrain rough to cross. Waste terrain with reduced defense but clear view. In Fog of War, unit's gain </r><div c='#00ff00'>vision +1.</div>");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("<r>In Fog of War conditions, the ruins provide ground unit </r><div c='#00ff00'>hiding places.</div><r> Snowy terrain rough to cross.</r>");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("<r>In Fog of War conditions, the ruins provide ground unit </r><div c='#00ff00'>hiding places.</div><r> It reduces the firepower of units by 20%.</r>");
        }
        else
        {
            return qsTr("<r>In Fog of War conditions, the ruins provide ground unit </r><div c='#00ff00'>hiding places.</div>");
        }
    };
    this.getSprites = function(spriteId)
    {
        return [spriteId + "+0",
                spriteId + "+1",
                spriteId + "+2"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return "back_wasteruin";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "back_snowruin";
        }
        else if (baseTerrainId === "DESERT")
        {
            return "back_desertruin";
        }
        else
        {
            var weatherModifier = TERRAIN.getWeatherModifier(map);
            return "back_" + weatherModifier + "ruin";
        }
    };
};
Constructor.prototype = TERRAIN;
var __BASERUIN = new Constructor();
