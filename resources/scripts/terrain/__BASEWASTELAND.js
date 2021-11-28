var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(__BASEWASTELAND.getName(terrain));
    };
    this.baseTerrainId = "PLAINS";
    this.getName = function(terrain)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return qsTr("Waste Wasteland");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Snowy Wasteland");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Desert Wasteland");
        }
        else
        {
            return qsTr("Wasteland");
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
            return 1;
        }
        else
        {
            return 2;
        }
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
    this.loadBaseTerrain = function(terrain, currentTerrainID)
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

    this.loadBase = function(terrain, spriteId)
    {
        var random = globals.randInt(0, 5);
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
            return "minimap_waste_wasteland";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "minimap_snow_wasteland";
        }
        else if (baseTerrainId === "DESERT")
        {
            return "minimap_desert_wasteland";
        }
        else
        {
            return "minimap_wasteland";
        }
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
            return qsTr("<r>Waste terrain with reduced defense but clear view. In Fog of War, unit's gain </r><div c='#00ff00'>vision +1.</div><r> Mobility is impaired on this rough terrain.</r>");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Mobility is impaired on this rough terrain.");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Infantry and Mech units aside, mobility is impaired on rough terrain. It reduces the firepower of units by 20%.");
        }
        else
        {
            return qsTr("Infantry and Mech units aside, mobility is impaired on rough terrain.");
        }
    };
    this.getSprites = function(spriteId)
    {
        return [spriteId + "+0",
                spriteId + "+1",
                spriteId + "+2",
                spriteId + "+3",
                spriteId + "+4",
                spriteId + "+5"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return "back_wastewasteland";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "back_snowwasteland";
        }
        else if (baseTerrainId === "DESERT")
        {
            return "back_desertwasteland";
        }
        else
        {
            return "back_wasteland";
        }
    };
};
Constructor.prototype = TERRAIN;
var __BASEWASTELAND = new Constructor();
