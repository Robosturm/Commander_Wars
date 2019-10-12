var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(RIVER.getName());
    };
    this.getName = function()
    {
        return qsTr("River");
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
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("RIVER,BRIDGE,SEA", false, false, GameEnums.Directions_Direct);
        var surroundingsSea = terrain.getSurroundings("SEA", true, false, GameEnums.Directions_All);
        //
        if (!(surroundingsSea.includes("+N+NE") && surroundingsSea.includes("+NW")))
        {
            var surroundingsN = terrain.getSurroundings("SEA", true, false, GameEnums.Directions_North);
            if (surroundingsN === "+N")
            {
                surroundings = surroundings.replace("+N", "");
            }
        }

        if (!(surroundingsSea.includes("SE+S+SW")))
        {
            var surroundingsS = terrain.getSurroundings("SEA", true, false, GameEnums.Directions_South);
            if (surroundingsS === "+S")
            {
                surroundings = surroundings.replace("+S", "");
            }
        }

        if ((surroundingsSea.includes("+E") ))
        {
            if (!(surroundingsSea.includes("NE+E+SE")))
            {
                surroundings = surroundings.replace("+E", "");
            }
        }
        if ((surroundingsSea.includes("+W") && !(surroundingsSea.includes("SW+W+NW"))))
        {
            surroundings = surroundings.replace("+W", "");
        }
        terrain.loadBaseSprite("river" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_river";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_river";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_river";
    };
    this.getDescription = function()
    {
        return qsTr("Most infantry and Hovercraft units are the only ground unit that can cross rivers.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["river",
                "river+E",
                "river+E+S",
                "river+E+S+W",
                "river+E+W",
                "river+N",
                "river+N+E",
                "river+N+E+S",
                "river+N+E+S+W",
                "river+N+E+W",
                "river+N+S",
                "river+N+S+W",
                "river+N+W",
                "river+S",
                "river+S+W",
                "river+W"];
    };
};
Constructor.prototype = TERRAIN;
var RIVER = new Constructor();
