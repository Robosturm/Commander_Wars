var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESERT.getName());
    };

    this.getName = function()
    {
        return qsTr("Desert");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain, currentTerrainID)
    {
        var random = globals.randInt(0, 140);
        if (random >= 8)
        {
            terrain.loadBaseSprite("desert+0");
        }
        else
        {
            terrain.loadBaseSprite("desert+" + random.toString());
        }
    };
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert";
    };
    this.loadOverlaySprite = function(terrain)
    {
        var surroundingsPlains = terrain.getSurroundings("PLAINS,WASTELAND,SEA", true, false, GameEnums.Directions_Direct, false);
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
        var surroundingsDesert = terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert.includes("+N"))
        {
            terrain.loadOverlaySprite("snow+N");
        }
        if (surroundingsDesert.includes("+E"))
        {
            terrain.loadOverlaySprite("snow+E");
        }
        if (surroundingsDesert.includes("+S"))
        {
            terrain.loadOverlaySprite("snow+S");
        }
        if (surroundingsDesert.includes("+W"))
        {
            terrain.loadOverlaySprite("snow+W");
        }
    };

    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain, but it reduces the firerange of indirect units by 1.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["desert+0",
                "desert+1",
                "desert+2",
                "desert+3",
                "desert+4",
                "desert+5",
                "desert+6",
                "desert+7"];
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender)
    {
        return TERRAIN.getFactoryForeground(terrain);
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain);
        switch (id)
        {
        case "DESERT_WELD":
        case "SNOW_WELD":
        case "WELD":
        case "PIPELINE":
        case "DESERT_PIPELINE":
        case "SNOW_PIPELINE":
            return "back_desert+pipe";
        default:
            return "back_desert";
        }
    };
};
Constructor.prototype = TERRAIN;
var DESERT = new Constructor();
