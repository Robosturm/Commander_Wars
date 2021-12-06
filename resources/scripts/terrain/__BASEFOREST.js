var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(__BASEFOREST.getName(terrain));
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
            return qsTr("Waste Forest");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("Snowy Forest");
        }
        else if (baseTerrainId === "DESERT")
        {
            return qsTr("Desert Forest");
        }
        else
        {
            return qsTr("Forest");
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

    this.loadBase = function(terrain, terrainId, spriteId)
    {
        var surroundings = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite(spriteId + surroundings);
    };
    this.loadOverlay = function(terrain, terrainId, spriteId)
    {
        // Check every side.
        var surroundings = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_Direct, false);
        // Load overlay south east, strict.
        if (surroundings.includes("+S") && surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite(spriteId + "+SE");
            }
        }
        // Load overlay north east, strict.
        if (surroundings.includes("+N") && surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite(spriteId + "+NE");
            }
        }
        // Load overlay south west, strict.
        if (surroundings.includes("+S") && surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite(spriteId + "+SW");
            }
        }
        // Load overlay northwest, strict.
        if (surroundings.includes("+N") && surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings(terrainId, false, false, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite(spriteId + "+NW");
            }
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
            return "minimap_waste_forest";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "minimap_snow_forest";
        }
        else if (baseTerrainId === "DESERT")
        {
            return "minimap_desert_forest";
        }
        else
        {
            return "minimap_forest";
        }
    };
    this.getVisionHide = function()
    {
        return true;
    };

    this.getDescription = function(terrain)
    {
        var baseTerrainId = terrain.getBaseTerrainID();
        if (baseTerrainId === "WASTE")
        {
            return qsTr("<r>In Fog of War conditions, the snowy woods provide ground unit </r><div c='#00ff00'>hiding places.</div><r> Waste terrain with reduced defense but clear view. In Fog of War, unit's gain </r><div c='#00ff00'>vision +1.</div>");
        }
        else if (baseTerrainId === "SNOW")
        {
            return qsTr("<r>In Fog of War conditions, the snowy woods provide ground unit </r><div c='#00ff00'>hiding places.</div><r> It's hard for ground units to cross this terrain.</r>");
        }
        else if (baseTerrainId === "DESERT")
        {
            return "<r>" + qsTr("In Fog of War conditions, the woods provide ground unit hiding places.") + "</r>" +
                    "<div c='#00ff00'>" + qsTr(" It reduces the firepower of units by 20%") + "</div>";
        }
        else
        {
            return qsTr("<r>In Fog of War conditions, the woods provide ground unit</r><div c='#00ff00'>hiding places.</div>");
        }
    };
    this.getSprites = function(spriteId)
    {
        return [spriteId,
                spriteId + "+E",
                spriteId + "+E+W",
                spriteId + "+W"];
    };

    this.getTerrainAnimationBase = function(unit, terrain)
    {        
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return "base_wasteforest";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "base_snowforest";
        }
        else if (baseTerrainId === "DESERT")
        {
            return "base_desertforest";
        }
        else
        {
            var weatherModifier = TERRAIN.getWeatherModifier();
            return "base_" + weatherModifier + "forest";
        }
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var variables = terrain.getVariables();
        var variable = variables.getVariable("BACKGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 2);
            variable = variables.createVariable("BACKGROUND_ID");
            variable.writeDataInt32(rand);
        }
        else
        {
            rand = variable.readDataInt32();
        }
        var baseTerrainId = ""
        if (terrain !== null)
        {
            baseTerrainId = Global[terrain.getTerrainID()].baseTerrainId;
        }
        if (baseTerrainId === "WASTE")
        {
            return "back_wasteforest";
        }
        else if (baseTerrainId === "SNOW")
        {
            return "back_snowforest+" + rand.toString();
        }
        else if (baseTerrainId === "DESERT")
        {
            return "back_desertforest+" + rand.toString();
        }
        else
        {
            var weatherModifier = TERRAIN.getWeatherModifier();
            return "back_" + weatherModifier + "forest+" + rand.toString();
        }
    };
};
Constructor.prototype = TERRAIN;
var __BASEFOREST = new Constructor();
