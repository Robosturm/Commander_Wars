var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(2);
        terrain.setTerrainName(__BASEPIPELINE.getName(terrain));
    };
    this.getName = function(terrain)
    {
        var baseTerrainId = terrain.getBaseTerrainID();
        if (baseTerrainId === "WASTE")
        {
            return qsTr("Waste Pipeline");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Snowy Pipeline");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Desert Pipeline");
        }
        else
        {
            return qsTr("Pipeline");
        }
    };
    this.getMovementcostModifier = function(terrain, unit, x, y, curX, curY)
    {
        var baseTerrainId = terrain.getBaseTerrainID();
        if (baseTerrainId === "SNOW")
        {
            return 1;
        }
        return 0;
    };
    this.getOffensiveFieldBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender, action, luckMode)
    {
        var baseTerrainId = terrain.getBaseTerrainID();
        if (baseTerrainId === "DESERT")
        {
            return -20;
        }
        return 0;
    };
    this.getBonusVision = function(unit)
    {
        var baseTerrainId = terrain.getBaseTerrainID();
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
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBase = function(terrain, spriteId)
    {
        var surroundings = terrain.getSurroundings("PIPELINE,WELD,DESTROYEDWELD,PIPESTATION,SNOW_PIPELINE,SNOW_DESTROYEDWELD,SNOW_WELD,DESERT_PIPELINE,DESERT_DESTROYEDWELD,DESERT_WELD,WASTE_PIPELINE,ZWELD_E_W,ZWELD_N_S,ZSNOWWELD_N_S,ZSNOWWELD_E_W,ZDESERTWELD_N_S,ZDESERTWELD_E_W", false, false, GameEnums.Directions_Direct, true, true);
        var x = terrain.getX();
        var y = terrain.getY();
        if (typeof map !== 'undefined')
        {
            if (map.onMap(x, y + 1))
            {
                var building = map.getTerrain(x, y + 1).getBuilding();
                if (building !== null &&
                        building.getBuildingID() === "ZBLACKHOLE_FACTORY" &&
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

        if (surroundings === "")
        {
            terrain.loadBaseSprite(spriteId + "+E+W");
        }
        else
        {
            terrain.loadBaseSprite(spriteId + surroundings);
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_pipeline";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_pipeline";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain);
        var baseTerrainId = terrain.getBaseTerrainID();
        if (baseTerrainId === "WASTE")
        {
            return "back_wasteplains";
        }
        else if (baseTerrainId === "SNOW")
        {
            return TERRAIN.getTerrainBackgroundId(id, "snow", true);
        }
        else if (baseTerrainId === "DESERT")
        {
            return TERRAIN.getTerrainBackgroundId(id, "desert", true);
        }
        else
        {
            var weatherModifier = TERRAIN.getWeatherModifier();
            return TERRAIN.getTerrainBackgroundId(id, weatherModifier, true);
        }
    };

    this.getDescription = function()
    {
        var baseTerrainId = terrain.getBaseTerrainID();
        if (baseTerrainId === "WASTE")
        {
            return qsTr("Black Hole Pipeline which can't be crossed by most units.");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Black Hole Pipeline which can't be crossed by most units and is also hard to cross for those who can.");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Black Hole Pipeline which can't be crossed by most units. It reduces the firepower of units by 20%.");
        }
        else
        {
            return qsTr("Black Hole Pipeline which can't be crossed by most units.");
        }
    };

    this.getSprites = function(spriteId)
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [spriteId + "+E",
                spriteId + "+E+S",
                spriteId + "+E+S+W",
                spriteId + "+E+W",
                spriteId + "+N",
                spriteId + "+N+E",
                spriteId + "+N+E+S",
                spriteId + "+N+E+S+W",
                spriteId + "+N+E+W",
                spriteId + "+N+S",
                spriteId + "+N+S+W",
                spriteId + "+N+W",
                spriteId + "+S",
                spriteId + "+S+W",
                spriteId + "+W"];
    };
};
Constructor.prototype = TERRAIN;
var __BASEPIPELINE = new Constructor();
