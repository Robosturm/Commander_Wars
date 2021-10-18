var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };

    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(BEACH.getName());
    };

    this.getName = function()
    {
        return qsTr("Beach");
    };

    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        terrain.loadBaseTerrain("SEA");
    };
    this.isSeaTile = function(x, y)
    {
        GameConsole.print("x=" + x + " y=" + y, 1);
        if (map.onMap(x, y))
        {
            if (map.getTerrain(x, y).getBaseTerrainID() === "SEA")
            {
                return true;
            }
        }
        else
        {
            return true;
        }
        return false;
    };
    this.loadBaseSprite = function (terrain)
    {
        var surroundingsLand = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct, false);
        var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, GameEnums.Directions_Direct, false);
        var frameTime = 150;
        if (surroundingsLand !== "")
        {
            if (surroundingsLand === "+S" && surroundingsBeach.includes("+N"))
            {
                surroundingsBeach = surroundingsBeach.replace("+N", "");
            }
            if (surroundingsLand === "+N" && surroundingsBeach.includes("+S"))
            {
                surroundingsBeach = surroundingsBeach.replace("+S", "");
            }
            if (surroundingsLand === "+E" && surroundingsBeach.includes("+W"))
            {
                surroundingsBeach = surroundingsBeach.replace("+W", "");
            }
            if (surroundingsLand === "+W" && surroundingsBeach.includes("+E"))
            {
                surroundingsBeach = surroundingsBeach.replace("+E", "");
            }
            terrain.loadBaseSprite("beach" + surroundingsBeach + "+land" + surroundingsLand, frameTime);
        }
        else
        {
            var x = terrain.getX();
            var y = terrain.getY();
            if (x >= 0 && y >= 0)
            {
                if (!BEACH.isSeaTile(x, y - 2))
                {
                    surroundingsBeach = surroundingsBeach.replace("+N", "");
                }
                if (!BEACH.isSeaTile(x, y + 2))
                {
                    surroundingsBeach = surroundingsBeach.replace("+S", "");
                }
                if (!BEACH.isSeaTile(x + 2, y))
                {
                    surroundingsBeach = surroundingsBeach.replace("+E", "");
                }
                if (!BEACH.isSeaTile(x - 2, y))
                {
                    surroundingsBeach = surroundingsBeach.replace("+W", "");
                }
                terrain.loadBaseSprite("beach" + surroundingsBeach, frameTime);
            }
            else
            {
                terrain.loadBaseSprite("beach");
            }
        }
    };
    this.canBePlaced = function (x, y) {
        var terrain = map.getTerrain(x, y);
        var surroundingsLand = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct, false);
        var surroundingsSea = terrain.getSurroundings("SEA", false, true, GameEnums.Directions_All, false);
        var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, GameEnums.Directions_Direct, false);
        // var c = (surroundingsLand === "" && surroundingsBeach === "");
        var c = (false);
        var d = (surroundingsLand.includes("+S") && surroundingsLand.includes("+E") && surroundingsSea.includes("+NW"));
        var e = (surroundingsLand.includes("+S") && surroundingsLand.includes("+W") && surroundingsSea.includes("+NE"));
        var f = (surroundingsLand.includes("+W") && surroundingsLand.includes("+S") && surroundingsSea.includes("+NE"));
        var g = (surroundingsLand.includes("+W") && surroundingsLand.includes("+N") && surroundingsSea.includes("+SE"));
        var h = (surroundingsLand.includes("+N") && surroundingsLand.includes("+W") && surroundingsSea.includes("+SE"));
        var i = (surroundingsLand.includes("+N") && surroundingsLand.includes("+E") && surroundingsSea.includes("+SW"));
        var j = (surroundingsLand.includes("+E") && surroundingsLand.includes("+S") && surroundingsSea.includes("+NW"));
        var k = (surroundingsLand.includes("+E") && surroundingsLand.includes("+N") && surroundingsSea.includes("+SW"));
        if (c) // || d || e || f || g || h || i || j || k)
        {
            return false;
        }
        else
        {
            return true;
        }
    };

    this.loadOverlaySprite = function(terrain)
    {
        var surroundingsSnow = terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow !== "")
        {
            terrain.loadOverlaySprite("sea+snow" + surroundingsSnow);
        }
        var surroundingsWaste = terrain.getSurroundings("WASTE", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsWaste !== "")
        {
            terrain.loadOverlaySprite("sea+waste" + surroundingsWaste);
        }
        var surroundingsDesert = terrain.getSurroundings("DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert !== "")
        {
            terrain.loadOverlaySprite("sea+desert" + surroundingsDesert);
        }

        // load river overlay
        var surroundings = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct, false);
        // load overlay south east
        if (!surroundings.includes("+S") && !surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSE);
            }
        }
        // load overlay north east
        if (!surroundings.includes("+N") && !surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNE);
            }
        }
        // load overlay south west
        if (!surroundings.includes("+S") && !surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSW);
            }
        }
        // load overlay north west
        if (!surroundings.includes("+N") && !surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNW);
            }
        }

        // var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, GameEnums.Directions_Direct, false);
        // if (surroundingsBeach.includes("+N"))
        // {
        //     terrain.loadOverlaySprite("sea+beach+N");
        // }
        // if (surroundingsBeach.includes("+S"))
        // {
        //     terrain.loadOverlaySprite("sea+beach+S");
        // }
        // if (surroundingsBeach.includes("+E"))
        // {
        //     terrain.loadOverlaySprite("sea+beach+E");
        // }
        // if (surroundingsBeach.includes("+W"))
        // {
        //     terrain.loadOverlaySprite("sea+beach+W");
        // }
    };

    this.getMiniMapIcon = function()
    {
        return "minimap_beach";
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        if (typeof defender !== 'undefined' &&
            defender !== null &&
            defender.getUnitType() === GameEnums.UnitType_Naval)
        {
            return "back_" + weatherModifier + "beach+sea";
        }
        return "back_" + weatherModifier + "beach";
    };

    this.getDescription = function()
    {
        return qsTr("Beachs over great landing places for landing ground troops.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["beach+E+land+N",
                "beach+E+land+N+S",
                "beach+E+land+N+S+W",
                "beach+E+land+N+W",
                "beach+E+land+S",
                "beach+E+land+S+W",
                "beach+E+S+land+N+W",
                "beach+E+W+land+N",
                "beach+E+W+land+N+S",
                "beach+E+W+land+S",
                "beach+land+E",
                "beach+land+E+S",
                "beach+land+E+S+W",
                "beach+land+E+W",
                "beach+land+N",
                "beach+land+N+E",
                "beach+land+N+E+S",
                "beach+land+N+E+S+W",
                "beach+land+N+E+W",
                "beach+land+N+S",
                "beach+land+N+S+W",
                "beach+land+N+W",
                "beach+land+S",
                "beach+land+S+W",
                "beach+land+W",
                "beach+N+E+land+S+W",
                "beach+N+land+E",
                "beach+N+land+E+S+W",
                "beach+N+land+E+W",
                "beach+N+land+E+S",
                "beach+N+land+S+W",
                "beach+N+land+W",
                "beach+N+S+land+E",
                "beach+N+S+land+E+W",
                "beach+N+S+land+W",
                "beach+N+W+land+E+S",
                "beach+S+land+E",
                "beach+S+land+W",
                "beach+S+land+E+W",
                "beach+S+land+N+E",
                "beach+S+land+N+E+W",
                "beach+S+land+N+W",
                "beach+W+land+E+S",
                "beach+W+land+N",
                "beach+W+land+N+E",
                "beach+S+W+land+N+E",
                "beach+W+land+N+E+S",
                "beach+W+land+N+S",
                "beach+W+land+S",
                "beach"];
    };
};
Constructor.prototype = TERRAIN;
var BEACH = new Constructor();
