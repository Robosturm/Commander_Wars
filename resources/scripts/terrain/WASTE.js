var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    this.getDefaultPalette = function()
    {
        return "palette_waste";
    };
    this.init = function (terrain)
    {
        if (terrain.getPalette() === "")
        {
            terrain.setPalette(WASTE.getDefaultPalette());
        }
        terrain.setTerrainName(WASTE.getName());
    };
    this.getName = function()
    {
        return qsTr("Waste");
    };
    this.getDefense = function()
    {
        return 0;
    };
    this.loadBaseSprite = function(terrain, map)
    {
		var random = globals.randInt(0, 140);
        if (random >= 8)
        {
            terrain.loadBaseSprite("waste+0");
        }
        else
        {
            terrain.loadBaseSprite("waste+" + random.toString());
        }
    };
    this.getBonusVision = function(unit, terrain)
    {
        return 1;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_waste";
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        var surroundingsPlains = terrain.getSurroundings("PLAINS", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsPlains.includes("+N"))
        {
            terrain.loadOverlaySprite("plains+N", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_North, "PLAINS"));
        }
        if (surroundingsPlains.includes("+E"))
        {
            terrain.loadOverlaySprite("plains+E", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_East, "PLAINS"));
        }
        if (surroundingsPlains.includes("+S"))
        {
            terrain.loadOverlaySprite("plains+S", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_South, "PLAINS"));
        }
        if (surroundingsPlains.includes("+W"))
        {
            terrain.loadOverlaySprite("plains+W", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_West, "PLAINS"));
        }
        var surroundingsSnow= terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow.includes("+N"))
        {
            terrain.loadOverlaySprite("snow+N", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_North, "SNOW"));
        }
        if (surroundingsSnow.includes("+E"))
        {
            terrain.loadOverlaySprite("snow+E", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_East, "SNOW"));
        }
        if (surroundingsSnow.includes("+S"))
        {
            terrain.loadOverlaySprite("snow+S", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_South, "SNOW"));
        }
        if (surroundingsSnow.includes("+W"))
        {
            terrain.loadOverlaySprite("snow+W", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_West, "SNOW"));
        }
        var surroundingsDesert = terrain.getSurroundings("DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert.includes("+N"))
        {
            terrain.loadOverlaySprite("desert+N", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_North, "DESERT"));
        }
        if (surroundingsDesert.includes("+E"))
        {
            terrain.loadOverlaySprite("desert+E", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_East, "DESERT"));
        }
        if (surroundingsDesert.includes("+S"))
        {
            terrain.loadOverlaySprite("desert+S", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_South, "DESERT"));
        }
        if (surroundingsDesert.includes("+W"))
        {
            terrain.loadOverlaySprite("desert+W", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_West, "DESERT"));
        }
        var x = terrain.getX();
        var y = terrain.getY();
        var highTerrain = terrain.getSurroundings(TERRAIN.getHighTerrains(), true, false, GameEnums.Directions_West, false);
        if (map.onMap(x - 1, y))
        {
            var building = map.getTerrain(x - 1, y).getBuilding();
            if (building !== null &&
                    building.getBuildingWidth() === 1 &&
                    building.getBuildingHeigth() === 1)
            {
                highTerrain = "+W";
            }
        }
        if (highTerrain !== "")
        {
            terrain.loadOverlaySprite("shadow_waste");
        }
    };
    this.getDescription = function()
    {
        return qsTr("<r>Waste terrain with reduced defense but clear view. In Fog of War, unit's gain </r><div c='#00ff00'>vision +1.</div>");
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        if (TERRAIN.isPipeline(TERRAIN.getTerrainAnimationId(terrain, map)))
        {
            return "back_wasteplains+pipe";
        }
        else
        {
            return "back_wasteplains";
        }
    };
    
    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["waste+0",
                "waste+1",
                "waste+2",
                "waste+3",
                "waste+4",
                "waste+5",
                "waste+6",
                "waste+7"];
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return ["shadow_waste",
                "plains+N",
                "plains+E",
                "plains+S",
                "plains+W",
                "snow+N",
                "snow+E",
                "snow+S",
                "snow+W",
                "desert+N",
                "desert+E",
                "desert+S",
                "desert+W",]
    };
};
Constructor.prototype = TERRAIN;
var WASTE = new Constructor();
