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
    this.loadBaseTerrain = function(terrain, currentTerrainID, map)
    {
        if (currentTerrainID === "LAKE")
        {
            terrain.loadBaseTerrain("LAKE");
        }
        else
        {
            terrain.loadBaseTerrain("SEA");
        }
    };
    this.loadBaseSprite = function(terrain, map)
    {
        var surroundingsPlainsDiagonal = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Diagnonal, false);
        var surroundingsPlainsDirect = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Direct, false);
        var fullName = "reaf" + surroundingsPlainsDiagonal + surroundingsPlainsDirect;
        if (terrain.existsResAnim(fullName))
        {
            terrain.loadBaseSprite(fullName);
        }
        else if (surroundingsPlainsDirect !== "")
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
    this.loadOverlaySprite = function(terrain, map)
    {
        var baseId = terrain.getBaseTerrainID();
        if (baseId === "SEA")
        {
            SEA.loadOverlaySprite(terrain, map);
        }
        else
        {
            LAKE.loadOverlaySprite(terrain, map);
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
    this.canBePlaced = function(x, y, map)
    {
        var terrain = map.getTerrain(x, y);
        var baseId = terrain.getBaseTerrainID();
        if (baseId === "SEA" ||
            baseId === "LAKE")
        {
            var diagonalCount = 0;
            var surroundingsPlainsDiagonal = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Diagnonal, false);
            var surroundingsPlainsDirect = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Direct, false);
            var fullName = "reaf" + surroundingsPlainsDiagonal + surroundingsPlainsDirect;
            if (terrain.existsResAnim(fullName))
            {
                return true;
            }
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
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
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
        return ["reaf+0+N+E+S+W",
                "reaf+1+N+E+S+W",
                "reaf+2+N+E+S+W",
                "reaf+3+N+E+S+W",
                "reaf+E.png",
                "reaf+N.png",
                "reaf+S.png",
                "reaf+W.png",
                "reaf+E+S",
                "reaf+N+E",
                "reaf+N+W",
                "reaf+S+W",
                "reaf+E+S+W",
                "reaf+E+W",
                "reaf+N+E+S",
                "reaf+N+E+W",
                "reaf+N+S+W",
                "reaf+N+S",
                "reaf+NE+NW+S",
                "reaf+NE+NW",
                "reaf+NE+S+W",
                "reaf+NE+S",
                "reaf+NE+W",
                "reaf+NE",
                "reaf+NW+E",
                "reaf+NW+S+E",
                "reaf+NW+S",
                "reaf+NW",];
    };
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
};
Constructor.prototype = TERRAIN;
var REAF = new Constructor();
