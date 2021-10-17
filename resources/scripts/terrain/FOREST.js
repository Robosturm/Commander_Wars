var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Forest");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var directSurroundings = terrain.getSurroundings("FOREST", false, false, GameEnums.Directions_Direct, false);
        var surroundings = terrain.getSurroundings("FOREST", false, false, GameEnums.Directions_All, false);
        if (surroundings !== "+E" &&
            surroundings !== "+W" &&
            surroundings !== "+E+W")
        {
            if (!(directSurroundings.includes("+S") && directSurroundings.includes("+E")))
            {
                surroundings = surroundings.replace("+SE", "");
            }
            if (!(directSurroundings.includes("+N") && directSurroundings.includes("+E")))
            {
                surroundings = surroundings.replace("+NE", "");
            }
            if (!(directSurroundings.includes("+N") && directSurroundings.includes("+W")))
            {
                surroundings = surroundings.replace("+NW", "");
            }
            if (!(directSurroundings.includes("+S") && directSurroundings.includes("+W")))
            {
                surroundings = surroundings.replace("+SW", "");
            }

            if (directSurroundings.includes("+S") &&
                (!surroundings.includes("+SW") &&
                 !surroundings.includes("+SE")))
            {
                surroundings = surroundings.replace("+S", "");
            }
            if (directSurroundings.includes("+N") &&
                (!surroundings.includes("+NW") &&
                 !surroundings.includes("+NE")))
            {
                surroundings = surroundings.replace("+N", "");
            }
            if (directSurroundings.includes("+E") &&
                (!surroundings.includes("+SE") &&
                 !surroundings.includes("+NE")))
            {
                surroundings = surroundings.replace("+E", "");
            }
            if (directSurroundings.includes("+W") &&
                (!surroundings.includes("+SW") &&
                 !surroundings.includes("+NW")))
            {
                surroundings = surroundings.replace("+W", "");
            }
        }
        if (terrain.existsResAnim("forest" + surroundings))
        {
            terrain.loadBaseSprite("forest" + surroundings);
        }
        else
        {
            GameConsole.print("missing sprite " + surroundings, 1);
            terrain.loadBaseSprite("forest");
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_forest";
    };
    this.getVisionHide = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War conditions, the woods provide ground unit</r><div c='#00ff00'>hiding places.</div>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["forest",
                "forest+N",
                "forest+S",
                "forest+E",
                "forest+W",
                "forest+N+S",
                "forest+E+W",
                "forest+E+SE+S",
                "forest+E+SE+S+SW+W",
                "forest+N+NE+E",
                "forest+N+NE+E+SE+S.png",
                "forest+N+NE+E+SE+S+SW+W+NW",
                "forest+N+NE+E+S+SW+W",
                "forest+N+NE+E+W+NW",
                "forest+N+S+SW+W+NW",
                "forest+S+SW+W",
                "forest+N+W+NW",
                "forest+N+E+SE+S+W+NW",
                "forest+N+E+SE+S+SW+W+NW",
                "forest+N+NE+E+SE+S+SW+W",
                "forest+N+NE+E+S+SW+W+NW",
                "forest+N+NE+E+SE+S+W+NW",];
    };

    this.getTerrainAnimationBase = function(unit, terrain)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "base_" + weatherModifier + "forest";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 2);
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "back_" + weatherModifier + "forest+" + rand.toString();
    };
};
Constructor.prototype = TERRAIN;
var FOREST = new Constructor();
