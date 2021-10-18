var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
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
    this.loadBaseSprite = function(terrain)
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
    this.getBonusVision = function(unit)
    {
        return 1;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_waste";
    };
    this.loadOverlaySprite = function(terrain)
    {
        var surroundingsPlains = terrain.getSurroundings("PLAINS", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsPlains.includes("+N"))
        {
            terrain.loadOverlaySprite("plains+N");
        }
        if (surroundingsPlains.includes("+E"))
        {
            terrain.loadOverlaySprite("plains+E");
        }
        if (surroundingsPlains.includes("+S"))
        {
            terrain.loadOverlaySprite("plains+S");
        }
        if (surroundingsPlains.includes("+W"))
        {
            terrain.loadOverlaySprite("plains+W");
        }
        var surroundingsSnow= terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow.includes("+N"))
        {
            terrain.loadOverlaySprite("snow+N");
        }
        if (surroundingsSnow.includes("+E"))
        {
            terrain.loadOverlaySprite("snow+E");
        }
        if (surroundingsSnow.includes("+S"))
        {
            terrain.loadOverlaySprite("snow+S");
        }
        if (surroundingsSnow.includes("+W"))
        {
            terrain.loadOverlaySprite("snow+W");
        }
        var surroundingsDesert = terrain.getSurroundings("DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert.includes("+N"))
        {
            terrain.loadOverlaySprite("desert+N");
        }
        if (surroundingsDesert.includes("+E"))
        {
            terrain.loadOverlaySprite("desert+E");
        }
        if (surroundingsDesert.includes("+S"))
        {
            terrain.loadOverlaySprite("desert+S");
        }
        if (surroundingsDesert.includes("+W"))
        {
            terrain.loadOverlaySprite("desert+W");
        }
    };
    this.getDescription = function()
    {
        return qsTr("<r>Waste terrain with reduced defense but clear view. In Fog of War, unit's gain </r><div c='#00ff00'>vision +1.</div>");
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_wasteplains";
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
};
Constructor.prototype = TERRAIN;
var WASTE = new Constructor();
