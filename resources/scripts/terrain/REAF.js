var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(REAF.getName());
    };
    this.getName = function()
    {
        return qsTr("Reaf");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundingsPlainsDiagonal = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Diagnonal, false);
        var surroundingsPlainsDirect = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsPlainsDirect !== "")
        {
            terrain.loadBaseSprite("reaf" + surroundingsPlainsDirect);
        }
        else if (surroundingsPlainsDiagonal !== "")
        {
            if (surroundingsPlainsDiagonal.includes("+NE") &&
                surroundingsPlainsDiagonal.includes("+SE") &&
                surroundingsPlainsDiagonal.includes("+SW") ||
                surroundingsPlainsDiagonal === "+SE")
            {
                terrain.loadBaseSprite("reaf+E+S");
            }
            else if (surroundingsPlainsDiagonal.includes("+SE") &&
                     surroundingsPlainsDiagonal.includes("+SW") &&
                     surroundingsPlainsDiagonal.includes("+NW") ||
                     surroundingsPlainsDiagonal === "+SW")
            {
                terrain.loadBaseSprite("reaf+S+W");
            }
            else if (surroundingsPlainsDiagonal.includes("+SW") &&
                     surroundingsPlainsDiagonal.includes("+NW") &&
                     surroundingsPlainsDiagonal.includes("+NE") ||
                     surroundingsPlainsDiagonal === "+NW")
            {
                terrain.loadBaseSprite("reaf+N+W");
            }
            else if (surroundingsPlainsDiagonal.includes("+NW") &&
                     surroundingsPlainsDiagonal.includes("+NE") &&
                     surroundingsPlainsDiagonal.includes("+SE") ||
                     surroundingsPlainsDiagonal === "+NE")
            {
                terrain.loadBaseSprite("reaf+N+E");
            }
            else if (surroundingsPlainsDiagonal.includes("+SE") &&
                surroundingsPlainsDiagonal.includes("+SW"))
            {
                terrain.loadBaseSprite("reaf+S");
            }
            else if (surroundingsPlainsDiagonal.includes("+NE") &&
                     surroundingsPlainsDiagonal.includes("+NW"))
            {
                terrain.loadBaseSprite("reaf+N");
            }
            else if (surroundingsPlainsDiagonal.includes("+NE") &&
                     surroundingsPlainsDiagonal.includes("+SE"))
            {
                terrain.loadBaseSprite("reaf+E");
            }
            else if (surroundingsPlainsDiagonal.includes("+NW") &&
                     surroundingsPlainsDiagonal.includes("+SW"))
            {
                terrain.loadBaseSprite("reaf+W");
            }
        }
        else
        {
            var random = globals.randInt(0, 3);
            terrain.loadBaseSprite("reaf+" + random.toString() + "+N+E+S+W");
        }
    };

    this.loadOverlaySprite = function(terrain)
    {
        // load coast overlay
        var surroundings = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct, false);
        // load overlay south east
        if (!surroundings.includes("+S") && !surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("sea+overlay" + surroundingsSE);
            }
        }
        // load overlay north east
        if (!surroundings.includes("+N") && !surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("sea+overlay" + surroundingsNE);
            }
        }
        // load overlay south west
        if (!surroundings.includes("+S") && !surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("sea+overlay" + surroundingsSW);
            }
        }
        // load overlay north west
        if (!surroundings.includes("+N") && !surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("sea+overlay" + surroundingsNW);
            }
        }
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        if (terrain.getBaseTerrainID() === "SEA")
        {
            var diagonalCount = 0;
            var surroundingsPlainsDiagonal = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Diagnonal, false);
            var surroundingsPlainsDirect = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Direct, false);
            if (surroundingsPlainsDiagonal.includes("+SE"))
            {
                diagonalCount++;
            }
            if (surroundingsPlainsDiagonal.includes("+SW"))
            {
                diagonalCount++;
            }
            if (surroundingsPlainsDiagonal.includes("+NE"))
            {
                diagonalCount++;
            }
            if (surroundingsPlainsDiagonal.includes("+NW"))
            {
                diagonalCount++;
            }
            var directCount = 0;
            if (surroundingsPlainsDirect.includes("+N"))
            {
                directCount++;
            }
            if (surroundingsPlainsDirect.includes("+S"))
            {
                directCount++;
            }
            if (surroundingsPlainsDirect.includes("+E"))
            {
                directCount++;
            }
            if (surroundingsPlainsDirect.includes("+W"))
            {
                directCount++;
            }
            if ((directCount <= 2 && diagonalCount <= 2) ||
                (directCount == 0 && diagonalCount <= 3))
            {
                return true;
            }
        }
        return false;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_reaf";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "back_" + weatherModifier +"sea";
    };
    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };

    this.getDescription = function()
    {
        return qsTr("<r>This spiky reaf provides ideal </r><div c='#00ff00'>hiding places</div><r> for naval units in Fog of War.</r>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["reaf+N+E+S+W",
                "reaf+1+N+E+S+W",
                "reaf+2+N+E+S+W",
                "reaf+3+N+E+S+W"];
    };
};
Constructor.prototype = TERRAIN;
var REAF = new Constructor();
