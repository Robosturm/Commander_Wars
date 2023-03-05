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
            return qsTr("Waste Destroyed Weld");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Snowy Destroyed Weld");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Desert Destroyed Weld");
        }
        else
        {
            return qsTr("Destroyed Weld");
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
        var surroundings = terrain.getSurroundings("PIPELINE,SNOW_PIPELINE,DESERT_PIPELINE,WASTE_PIPELINE", false, false, GameEnums.Directions_Direct, true);
        var x = terrain.getX();
        var y = terrain.getY();
        if (map !== null)
        {
            if (map.onMap(x, y + 1))
            {
                var building = map.getTerrain(x, y + 1).getBuilding();
                if (building !== null &&
                   (building.getBuildingID() === "ZBLACKHOLE_FACTORY" ||
                    building.getBuildingID() === "ZBLACKHOLE_FACTORYDESERT" ||
                    building.getBuildingID() === "ZBLACKHOLE_FACTORYWASTE" ||
                    building.getBuildingID() === "ZBLACKHOLE_FACTORYSNOW") &&
                    building.getX() - 1 === x && building.getY() - 4 === y)
                {
                    if (surroundings.indexOf("+W") >= 0)
                    {
                        surroundings = surroundings.replace("+W", "+S+W");
                    }
                    else
                    {
                        surroundings += "+S";
                    }
                }
            }
        }
        if ((surroundings === ""))
        {
            terrain.loadBaseSprite(spriteId + "+E+W");
        }
        else if ((surroundings === "+N+S"))
        {

            terrain.loadBaseSprite(spriteId + "+N+S");
        }
        else if ((surroundings === "+E+W"))
        {
            terrain.loadBaseSprite(spriteId + "+E+W");
        }
        else
        {
            terrain.loadBaseSprite(spriteId + "+E+W");
        }
    };
    this.canBePlaced = function(x, y, map)
    {
        var terrain = map.getTerrain(x, y);
        var surroundings = terrain.getSurroundings("PIPELINE,DESERT_PIPELINE,SNOW_PIPELINE,WASTE_PIPELINE", false, false, GameEnums.Directions_Direct, true);
        if ((surroundings === "+E+W") || (surroundings === "+N+S"))
        {
            return true;
        }
        else
        {
            return false;
        }
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
            return "minimap_waste";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "minimap_snow";
        }
        else if (baseTerrainId === "DESERT")
        {
            return "minimap_desert";
        }
        else
        {
            return "minimap_plains";
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
            return qsTr("<r>Waste terrain with reduced defense but clear view. In Fog of War, unit's gain </r><div c='#00ff00'>vision +1.</div>");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Snowy terrain reduces rough to cross.");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Ground units move easily on this terrain, but it reduces the firepower of units by 20%");
        }
        else
        {
            return qsTr("Ground units move easily on this terrain.");
        }
    };
    this.getSprites = function()
    {
        return [spriteId + "+E+W",
                spriteId + "+N+S"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return WASTE.getTerrainAnimationForeground(unit, terrain, defender, map)
        }
        else if (baseTerrainId === "SNOW")
        {
            return SNOW.getTerrainAnimationForeground(unit, terrain, defender, map)
        }
        else if (baseTerrainId === "DESERT")
        {
            return DESERT.getTerrainAnimationForeground(unit, terrain, defender, map)
        }
        else
        {
            return TERRAIN.getTerrainAnimationForeground(unit, terrain, defender, map);
        }
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
            return WASTE.getTerrainAnimationBackground(unit, terrain, defender, map)
        }
        else if (baseTerrainId === "SNOW")
        {
            return SNOW.getTerrainAnimationBackground(unit, terrain, defender, map)
        }
        else if (baseTerrainId === "DESERT")
        {
            return DESERT.getTerrainAnimationBackground(unit, terrain, defender, map)
        }
        else
        {
            return TERRAIN.getTerrainAnimationBackground(unit, terrain, defender, map);
        }
    };
    this.getEditorPlacementSound = function()
    {
        return "placeBuilding.wav";
    };
};
Constructor.prototype = TERRAIN;
var __BASEDESTROYED_WELD = new Constructor();
